#include "Instance.h"
#include "Permutation.h"
#include "Digraph.h"
#include <algorithm>
#include <istream>

std::uint32_t PlacedRectangle::x_max() const {
    return x_min + width;
}

std::uint32_t PlacedRectangle::y_max() const {
    return y_min + height;
}

bool PlacedRectangle::intersects_open(PlacedRectangle const& other) const {
    bool const intersect_x = x_min < other.x_max() and other.x_min < x_max();
    bool const intersect_y = y_min < other.y_max() and other.y_min < y_max();
    return intersect_x and intersect_y;
}

bool PlacedRectangle::contains(PlacedRectangle const& other) const {
    return x_min <= other.x_min and y_min <= other.y_min and
        other.x_max() <= x_max() and other.y_max() <= y_max();
}

Instance::Instance(Rectangle const& chip_area): _chip_area(chip_area) {}

void Instance::add_circuit(Rectangle const& circuit) {
    _to_place.push_back(circuit);
}

std::optional<Digraph::CostVectorRef> Instance::compute_axis_coords(
    Permutation const& pi_inverse,
    std::vector<TempEdge> const& edges,
    Digraph& graph_instance,
    bool is_y
) const {
    for (auto const&[pi_of_i, pi_of_j] : edges) {
        graph_instance.add_edge(pi_inverse[pi_of_i], pi_inverse[pi_of_j]);
    }
    return graph_instance.compute_longest_paths(
        pi_inverse, is_y ? _chip_area.height : _chip_area.width
    );
}

auto Instance::compute_temp_edges(
    Permutation const& pi_inverse_then_rho
) const -> std::pair<std::vector<TempEdge>, std::vector<TempEdge>> {
    std::vector<TempEdge> x_edges;
    std::vector<TempEdge> y_edges;
    for (std::size_t pi_of_i = 0; pi_of_i < _to_place.size(); ++pi_of_i) {
        for (std::size_t pi_of_j = pi_of_i + 1; pi_of_j < _to_place.size(); ++pi_of_j) {
            auto const north_west = pi_inverse_then_rho[pi_of_i] < pi_inverse_then_rho[pi_of_j];
            if (north_west) {
                x_edges.push_back({pi_of_i, pi_of_j});
            } else {
                y_edges.push_back({pi_of_i, pi_of_j});
            }
        }
    }
    return {x_edges, y_edges};
}

std::optional<Solution> Instance::place() const {
    Digraph x_graph{_to_place.size()};
    Digraph y_graph{_to_place.size()};
    for (std::size_t i = 0; i < _to_place.size(); ++i) {
        x_graph.set_outgoing_edge_cost(i, _to_place.at(i).width);
        y_graph.set_outgoing_edge_cost(i, _to_place.at(i).height);
    }
    for (auto const& pi_inverse_then_rho : Permutations{_to_place.size()}) {
        auto const [x_edges, y_edges] = compute_temp_edges(pi_inverse_then_rho);
        for (auto const& pi_inverse : Permutations{_to_place.size()}) {
            auto const x_coords = compute_axis_coords(pi_inverse, x_edges, x_graph, false);
            if (x_coords) {
                auto const y_coords = compute_axis_coords(pi_inverse, y_edges, y_graph, true);
                if (y_coords) {
                    //return make_solution(*x_coords, *y_coords);
                }
            }
            x_graph.reset();
            y_graph.reset();
        }
    }
    return std::nullopt;
}

Solution Instance::make_solution(
        std::vector<std::uint32_t> const& x_coords,
        std::vector<std::uint32_t> const& y_coords
) const {
    Solution result;
    for (std::size_t i = 0; i < _to_place.size(); ++i) {
        result.push_back({_to_place.at(i), x_coords.at(i), y_coords.at(i)});
    }
    return result;
}

std::optional<Rectangle> read_rect(std::istream& input) {
    Rectangle result;
    input >> result.width >> result.height;
    if (input) {
        return result;
    } else {
        return std::nullopt;
    }
}

std::optional<Instance> Instance::from_file(std::istream& input) {
    auto const chip_area = read_rect(input);
    if (not chip_area) {
        return std::nullopt;
    }
    Instance result{*chip_area};
    std::size_t num_rects;
    input >> num_rects;
    if (not input) {
        return std::nullopt;
    }
    for (std::size_t i = 0; i < num_rects; ++i) {
        auto const new_circuit = read_rect(input);
        if (not new_circuit) {
            return std::nullopt;
        }
        result.add_circuit(*new_circuit);
    }
    return result;
}

PlacedRectangle Instance::get_chip_area() const {
    return {_chip_area, 0, 0};
}

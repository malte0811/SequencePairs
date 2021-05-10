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

template<bool y>
std::optional<Digraph::CostVectorRef> Instance::compute_axis_coords(
    Permutation const& pi, Permutation const& rho, Digraph& graph_instance
) const {
    auto const get_size = [](Rectangle const& r) {
        if constexpr (y) {
            return r.height;
        } else {
            return r.width;
        }
    };
    for (std::size_t i = 0; i < _to_place.size(); ++i) {
        //TODO reduce useless iterations(?)
        for (std::size_t j = 0; j < _to_place.size(); ++j) {
            if (i == j) { continue; }
            auto const south_west = pi.at(i) < pi.at(j);
            auto const north_west = rho.at(i) < rho.at(j);
            if (south_west and (north_west != y)) {
                graph_instance.add_edge(i, j, get_size(_to_place.at(i)));
            }
        }
    }
    auto result_opt = graph_instance.compute_longest_paths();
    if (not result_opt) {
        return std::nullopt;
    }
    Digraph::CostVector const& result = result_opt.value();
    for (std::size_t i = 0; i < result.size(); ++i) {
        if (result.at(i) + get_size(_to_place.at(i)) > get_size(_chip_area)) {
            return std::nullopt;
        }
    }
    return result_opt;
}

std::optional<Solution> Instance::place() const {
    Digraph x_graph{_to_place.size()};
    Digraph y_graph{_to_place.size()};
    for (auto const pi : Permutations{_to_place.size()}) {
        for (auto const rho : Permutations{_to_place.size()}) {
            auto const x_coords = compute_axis_coords<false>(pi, rho, x_graph);
            if (x_coords) {
                auto const y_coords = compute_axis_coords<true>(pi, rho, y_graph);
                if (y_coords) {
                    return make_solution(*x_coords, *y_coords);
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

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

/**
 * Computes the pair of x- and y-graphs occuring in the longest path problems resulting from the
 * given \rho(\pi^{-1}), where graph vertices are indexed by \pi(v) rather than v itself. This has
 * two advantages: The natural order of the vertex indices forms a topological order, and the graph
 * structure only depends on the permutation from the "outer" loop, so we only need to set edge
 * weights in the inner loop (and compute shortest paths).
 */
std::pair<Digraph, Digraph> Instance::compute_graphs_with_pi_indices(
    Permutation const& rho_of_pi_inverse
) const {
    Digraph x_graph(_to_place.size());
    Digraph y_graph(_to_place.size());
    for (std::size_t pi_of_i = 0; pi_of_i < _to_place.size(); ++pi_of_i) {
        for (std::size_t pi_of_j = pi_of_i + 1; pi_of_j < _to_place.size(); ++pi_of_j) {
            // pi(i) is already smaller than pi(j), so only need to compare rho(i)/rho(j)
            auto const north_west = rho_of_pi_inverse[pi_of_i] < rho_of_pi_inverse[pi_of_j];
            if (north_west) {
                x_graph.add_edge(pi_of_i, pi_of_j);
            } else {
                y_graph.add_edge(pi_of_i, pi_of_j);
            }
        }
    }
    return {std::move(x_graph), std::move(y_graph)};
}

std::optional<Solution> Instance::place() const {
    for (auto const& rho_of_pi_inverse : Permutations{_to_place.size()}) {
        // Graph vertex IDs correspond to pi(circuit_id)
        auto [x_graph, y_graph] = compute_graphs_with_pi_indices(rho_of_pi_inverse);
        for (auto const& pi : Permutations{_to_place.size()}) {
            // Set correct edge weights
            for (std::size_t i = 0; i < _to_place.size(); ++i) {
                x_graph.set_outgoing_edge_cost(pi.at(i), _to_place.at(i).width);
                y_graph.set_outgoing_edge_cost(pi.at(i), _to_place.at(i).height);
            }
            // Compute circuit positions
            auto const x_coords = x_graph.compute_longest_paths(get_chip_area().width);
            if (not x_coords) {
                continue;
            }
            auto const y_coords = y_graph.compute_longest_paths(get_chip_area().height);
            if (y_coords) {
                // If feasibile coordinates exist for both x and y, return the resulting (feasible)
                // solution. The coordinate vectors are indexed by vertex IDs, so by pi(circuit_id),
                // so we need to account for that
                return make_solution(pi, *x_coords, *y_coords);
            }
        }
    }
    return std::nullopt;
}

Solution Instance::make_solution(
    Permutation const& pi,
    std::vector<std::uint32_t> const& x_coords_by_pi,
    std::vector<std::uint32_t> const& y_coords_by_pi
) const {
    Solution result;
    for (std::size_t i = 0; i < _to_place.size(); ++i) {
        result.push_back({
            .x_min = x_coords_by_pi.at(pi.at(i)),
            .y_min = y_coords_by_pi.at(pi.at(i)),
            .width = _to_place.at(i).width,
            .height = _to_place.at(i).height,
        });
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
    return {
        .x_min = 0,
        .y_min = 0,
        .width = _chip_area.width,
        .height = _chip_area.height,
    };
}

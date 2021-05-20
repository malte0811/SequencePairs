#ifndef INSTANCE_H
#define INSTANCE_H

#include <cstdint>
#include <vector>
#include <optional>
#include <iosfwd>
#include "Permutation.h"
#include "Digraph.h"

struct Rectangle {
    std::uint32_t width;
    std::uint32_t height;
};

struct PlacedRectangle {
    std::uint32_t x_min;
    std::uint32_t y_min;
    std::uint32_t width;
    std::uint32_t height;

    std::uint32_t x_max() const;

    std::uint32_t y_max() const;

    bool intersects_open(PlacedRectangle const& other) const;

    bool contains(PlacedRectangle const& other) const;
};

using Solution = std::vector<PlacedRectangle>;

class Instance {
public:
    static std::optional<Instance> from_file(std::istream& input);

    Instance(Rectangle const& chip_area);

    void add_circuit(Rectangle const& circuit);

    std::optional<Solution> place() const;

    PlacedRectangle get_chip_area() const;
private:
    std::pair<Digraph, Digraph> compute_graphs_with_pi_indices(
        Permutation const& rho_of_pi_inverse
    ) const;

    Solution make_solution(
        Permutation const& pi,
        std::vector<std::uint32_t> const& x_coords_by_pi,
        std::vector<std::uint32_t> const& y_coords_by_pi
    ) const;

    /**
     * Computes the longest unweighted length of a path in the x/y graph used in the
     * sequence pairs algorithm. The weighted length of a path with k edges is at
     * least the size of the k smallest circuits. If this already exceeds the size
     * of our chip the graph for this axis cannot contain a path of length k.
     */
    std::size_t max_path_length(std::uint32_t Rectangle::* axis_size) const;

    bool is_unweighted_path_ok(
            Digraph& axis_graph, std::size_t max_axis_length
    ) const;

    std::vector<Rectangle> _to_place;
    Rectangle _chip_area;
};

#endif

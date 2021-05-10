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

struct PlacedRectangle : public Rectangle {
    std::uint32_t x_min;
    std::uint32_t y_min;

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
    template<bool y>
    std::optional<Digraph::CostVectorRef> compute_axis_coords(
        Permutation const& pi_inverse,
        Permutation const& pi_inverse_then_rho,
        Digraph& graph_instance
    ) const;

    Solution make_solution(
            std::vector<std::uint32_t> const& x_coords,
            std::vector<std::uint32_t> const& y_coords
    ) const;

    std::vector<Rectangle> _to_place;
    Rectangle _chip_area;
};

#endif

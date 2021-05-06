#ifndef INSTANCE_H
#define INSTANCE_H

#include <cstdint>
#include <vector>
#include <optional>
#include <iosfwd>
#include "Permutation.h"

struct Rectangle {
    std::uint32_t width;
    std::uint32_t height;
};

struct PlacedRectangle : public Rectangle {
    std::uint32_t x_min;
    std::uint32_t y_min;

    bool intersects_open(PlacedRectangle const& other) const {
        bool const intersect_x = x_min < other.x_min + other.width and other.x_min < x_min + width;
        bool const intersect_y = y_min < other.y_min + other.height and other.y_min < y_min + height;
        return intersect_x and intersect_y;
    }
};

using Solution = std::vector<PlacedRectangle>;

class Instance {
public:
    static std::optional<Instance> from_file(std::istream& input);

    Instance(Rectangle const& chip_area);

    void add_circuit(Rectangle const& circuit);

    std::optional<Solution> place() const;
private:
    template<bool y>
    std::optional<std::vector<std::uint32_t>> compute_axis_coords(
            Permutation const& pi, Permutation const& rho
    ) const;

    Solution make_solution(
            std::vector<std::uint32_t> const& x_coords,
            std::vector<std::uint32_t> const& y_coords
    ) const;

    std::vector<Rectangle> _to_place;
    Rectangle _chip_area;
};

#endif

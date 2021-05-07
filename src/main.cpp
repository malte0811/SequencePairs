#include "Instance.h"
#include <iostream>

int main() {
    auto const inst = Instance::from_file(std::cin);
    if (not inst) {
        return 1;
    }
    auto const solution = inst->place();
    if (solution) {
        for (auto const& rect_a : *solution) {
            for (auto const& rect_b : *solution) {
                if (&rect_a != &rect_b and rect_a.intersects_open(rect_b)) {
                    std::cerr << "Intersecting rectangles in output???\n";
                    return 1;
                }
            }
            if (not inst->get_chip_area().contains(rect_a)) {
                std::cerr << "Rectangle outside chip area???\n";
                return 1;
            }
        }
        for (auto const& placed_rect : *solution) {
            std::cout << placed_rect.x_min << ' ' << placed_rect.y_min << '\n';
        }
    } else {
        std::cout << "no feasible placement exists\n";
    }
}

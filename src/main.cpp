#include "Instance.h"
#include <iostream>
#include <fstream>

int main(int argc, char const** argv) {
    if (argc != 2) {
        std::cerr << "Expected exactly one argument\n";
        return 1;
    }
    std::ifstream in(argv[1]);
    auto const inst = Instance::from_file(in);
    in.close();
    if (not inst) {
        return 1;
    }
    auto const solution = inst->place();
    if (solution) {
        for (auto const& rect_a : *solution) {
            for (auto const& rect_b : *solution) {
                if (&rect_a != &rect_b and rect_a.intersects_open(rect_b)) {
                    std::cerr << "Intersecting rectangles in output!\n";
                }
            }
            if (not inst->get_chip_area().contains(rect_a)) {
                std::cerr << "Rectangle outside chip area!\n";
            }
        }
        for (auto const& placed_rect : *solution) {
            std::cout << placed_rect.x_min << ' ' << placed_rect.y_min << '\n';
        }
    } else {
        std::cout << "no feasible placement exists\n";
    }
}

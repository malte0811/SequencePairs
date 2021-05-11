#include "Permutation.h"
#include <algorithm>
#include <numeric>

Permutations::Permutations(std::size_t num_elements): 
    _num_elements(num_elements) {}

PermutationIterator::PermutationIterator(std::size_t const num_elements): _last_order(num_elements) {
    std::iota(_last_order.begin(), _last_order.end(), 0);
}

PermutationIterator& PermutationIterator::operator++() {
    _done = not std::next_permutation(_last_order.begin(), _last_order.end());
    return *this;
}

#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <cstdint>
#include <vector>

class PermutationIterator;

using Permutation = std::vector<std::size_t>;

class Permutations {
public:
    struct EndIterator{};

    Permutations(std::size_t num_elements);

    PermutationIterator begin() const;

    EndIterator end() const;
private:
    std::size_t _num_elements;
};

class PermutationIterator {
public:
    PermutationIterator& operator++();

    Permutation const& operator*() const;

    bool operator!=(Permutations::EndIterator);
private:
    friend Permutations;

    PermutationIterator(std::size_t num_elements);

    Permutation _last_order;
    bool _done = false;
};

#endif
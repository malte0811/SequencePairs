#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <cstdint>
#include <vector>

class PermutationIterator;

using Permutation = std::vector<std::size_t>;

/// Helper to cleanly iterate over all permutations of {0, …, k}.
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

// INLINE SECTION

inline PermutationIterator Permutations::begin() const {
    return PermutationIterator{_num_elements};
}

inline auto Permutations::end() const -> EndIterator {
    return EndIterator{};
}

inline bool PermutationIterator::operator!=(Permutations::EndIterator) {
    return not _done;
}

inline Permutation const& PermutationIterator::operator*() const {
    return _last_order;
}

#endif

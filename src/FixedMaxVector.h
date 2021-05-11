#ifndef FIXED_MAX_VECTOR_H
#define FIXED_MAX_VECTOR_H

#include <vector>
#include <cstdint>
#include <type_traits>
#include <cassert>

template<typename T>
concept TrivialType = std::is_trivial_v<T>;

template<TrivialType T>
class FixedMaxVector {
public:
    FixedMaxVector(std::size_t size):
        _storage(size),
        _num_elements(0) {}

    void push_back(T const& to_add) {
        assert(_num_elements < _storage.size());
        _storage[_num_elements] = to_add;
        ++_num_elements;
    }

    void reset() {
        _num_elements = 0;
    }

    auto begin() const {
        return _storage.begin();
    }

    auto end() const {
        return _storage.begin() + _num_elements;
    }
private:
    std::vector<T> _storage;
    std::size_t _num_elements;
};

#endif

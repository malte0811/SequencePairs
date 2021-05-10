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
    FixedMaxVector(std::size_t size);

    void push_back(T const& to_add);

    T pop_back();

    void reset();

    std::size_t size() const;

    T const& at(std::size_t index) const;

    T& at(std::size_t index);

    bool empty() const;

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

template<TrivialType T>
FixedMaxVector<T>::FixedMaxVector(std::size_t size):
    _storage(size),
    _num_elements(0) {}

template<TrivialType T>
void FixedMaxVector<T>::push_back(T const& to_add) {
    assert(_num_elements < _storage.size());
    _storage[_num_elements] = to_add;
    ++_num_elements;
}

template<TrivialType T>
T FixedMaxVector<T>::pop_back() {
    assert(_num_elements > 0);
    --_num_elements;
    return _storage[_num_elements];
}

template<TrivialType T>
void FixedMaxVector<T>::reset() {
    _num_elements = 0;
}

template<TrivialType T>
std::size_t FixedMaxVector<T>::size() const {
    return _num_elements;
}

template<TrivialType T>
bool FixedMaxVector<T>::empty() const {
    return size() == 0;
}

template<TrivialType T>
T const& FixedMaxVector<T>::at(std::size_t index) const {
    assert(index < _num_elements);
    return _storage[index];
}

template<TrivialType T>
T& FixedMaxVector<T>::at(std::size_t index) {
    assert(index < _num_elements);
    return _storage[index];
}

#endif

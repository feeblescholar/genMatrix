/**
 * @file vector_assignment.ipp
 * @author Kovács Botond
 * @brief Vektorok értékadásainak implementáció.
 */
#ifndef VECTOR_ASSIGNMENT_I
#define VECTOR_ASSIGNMENT_I

#include "vector.hpp"

namespace genMatrix {

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this != &other)
        *this = this->operator=<T>(other);

    return *this;
}

template<typename T>
template<typename S>
Vector<T>& Vector<T>::operator=(const Vector<S>& other) {
    if (data) 
        delete[] data;

    data = new T[other.capacity()];
    _capacity = other.capacity();
    _size = other.size();

    for (size_t i = 0; i < _size; i++) {
        data[i] = other[i];
    }

    return *this;
}

}

#endif

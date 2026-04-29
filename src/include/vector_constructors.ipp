/**
 * @file vector_constructors.ipp
 * @author Kovács Botond
 * @brief Vektor konstruktor, copy konstruktor, move konstuktor és destruktor
 *        definíciók.
 */
#ifndef VECTOR_I
#define VECTOR_I

#include "vector.hpp"

namespace genMatrix {

template<typename T>
Vector<T>::Vector(size_t n) : data(nullptr), _size(0), _capacity(n) {
    if (n == 0)
        throw Vector_Error("[Constructor]", "Initial capacity must be >=1");
    data = new T[n];
}

template<typename T>
Vector<T>::Vector(const Matrix<T>& mtx, const size_t idx) 
    : data(nullptr)
    , _size(mtx.getRows()) 
    , _capacity(mtx.getRows()) 
{
    data = new T[_size];
    for (size_t i = 0; i < _size; i++)
        data[i] = mtx(i, idx);
}

template<typename T>
Vector<T>::Vector(const Vector<T>& other) 
    : data(nullptr) 
    , _size(other._size)
    , _capacity(other._capacity) 
{
    *this = other;
}

template<typename T>
template<typename S>
Vector<T>::Vector(const Vector<S>& other) 
    : data(nullptr) 
    , _size(other._size)
    , _capacity(other._capacity) 
{
    *this = other;
}

template<typename T>
Vector<T>::Vector(Vector<T>&& other) : data(nullptr), _size(0), _capacity(0) {
    data = std::exchange(other.data, nullptr);
    _size = std::exchange(other._size, 0);
    _capacity = std::exchange(other._capacity, 0);
}

template<typename T>
Vector<T>::~Vector() {
    if (data) 
        delete[] data;
    
    data = nullptr;
    _size = _capacity = 0;
}
}

#endif

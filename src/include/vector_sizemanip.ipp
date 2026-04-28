/**
 * @file vector_sizemanip.ipp
 * @author Kovács Botond
 * @brief Vektorok méretézéséhez szükséges tagfüggvények implementációi.
 */
#ifndef VECTOR_SIZEMANIP_I
#define VECTOR_SIZEMANIP_I

#include "vector.hpp"

namespace genMatrix {

template<typename T>
Vector<T>& Vector<T>::push_back(const T& val) {
    if (_size + 1 > _capacity) {
        T* ndata = new T[_capacity * 2];
        for (size_t i = 0; i < _size; i++)
            ndata[i] = data[i];

        _capacity *= 2;
        delete[] data;
        data = ndata;
    }

    data[_size++] = val;
    return *this;
}

template<typename T>
void Vector<T>::resize(const size_t ncap) {
    if (ncap < _size)
        throw "hibaosztaly ide";
    
    if (ncap == _capacity)
        return;

    T* ndata = new T[ncap];
    if (_size != 0) 
        for (size_t i = 0; i < _size; i++)
            ndata[i] = data[i];
    
    _capacity = ncap;
    delete[] data;
    data = ndata;
}

template<typename T>
void Vector<T>::shrink() {
    if (_size == 0) {
        delete[] data;
        data = nullptr;
        _capacity = 0;
        return;
    }

    T* ndata = new T[_size];
    for (size_t i = 0; i < _size; i++)
        ndata[i] = data[i];
    
    _capacity = _size;
    delete[] data;
    data = ndata;
}
}
    
#endif

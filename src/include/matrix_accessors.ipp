/**
 * @file matrix_accessors.ipp
 * @author Kovács Botond
 * @brief Mátrix indexelő operátor definició.
 */
#ifndef MATRIX_ACCESSORS_I
#define MATRIX_ACCESSORS_I

#include "matrix.hpp"

namespace genMatrix {

template<typename T>
const T& Matrix<T>::operator()(const size_t row, const size_t col) const {
    if (!n && !m) 
        throw Matrix_Error("[operator()]", "The matrix is empty.");

    if (row >= n || col >= m) 
        throw Matrix_Error("[operator()]", "Out of index");

    return data[row * m + col];
}

template<typename T>
T& Matrix<T>::operator()(const size_t row, const size_t col) {
    if (!n && !m) 
        throw Matrix_Error("[operator()]", "The matrix is empty.");

    if (row >= n || col >= m) 
        throw Matrix_Error("[operator()]", "Out of index");

    return data[row * m + col];
}
}

#endif

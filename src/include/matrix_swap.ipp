/**
 * @file matrix_accessors.ipp
 * @author Kovács Botond
 * @brief Mátrix sor- és oszlopcsere implementáció.
 */
#ifndef MATRIX_SWAP_I
#define MATRIX_SWAP_I

#include "matrix.hpp"

namespace genMatrix {

template<typename T>
Matrix<T>& Matrix<T>::swapRow(size_t n1, size_t n2) {
    for (size_t i = 0; i < m; i++) {
        swap(&this->operator()(n1, i), &this->operator()(n2, i));
    }
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::swapCol(size_t m1, size_t m2) {
    for (size_t i = 0; i < n; i++) {
        swap(&this->operator()(i, m1), &this->operator()(i, m2));
    }
    return *this;
}
}

#endif

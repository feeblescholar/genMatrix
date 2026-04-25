/**
 * @file matrix_assignment.ipp
 * @author Kovács Botond
 * @brief Mátrix traszponálás logikája.
 */
#ifndef MATRIX_TRANSPOSE_I
#define MATRIX_TRANSPOSE_I

#include "matrix.hpp"

namespace genMatrix {

template<typename T>
void Matrix<T>::transposeInPlace() {
    if (n == m) {
        for (size_t i = 0; i < n; i++)
            for (size_t j = i + 1; j < m; j++)
                swap(&this->operator()(i,j), &this->operator()(j, i));
    }
    else {
        Matrix<T> tmp(m, n);
        for (size_t i = 0; i < n; i++)
            for (size_t j = 0; j < m; j++)
                tmp(j, i) = this->operator()(i, j);
        *this = tmp;
    }
}

template<typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> tmp = *this;
    tmp.transposeInPlace();
    return tmp;
}
}

#endif

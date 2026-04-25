/**
 * @file matrix_binaryop.ipp
 * @author Kovács Botond
 * @brief Mátrixok közötti vagy balértéken végzett bináris operátorok definíciói.
 */
#ifndef MATRIX_BINARYOP_I
#define MATRIX_BINARYOP_I

#include "matrix.hpp"

namespace genMatrix {

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs_mtx) const {
    Matrix<T> rval = *this;
    rval += rhs_mtx;
    return rval;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const T& rhs_type) const {
    Matrix<T> rval = *this;
    rval += rhs_type;
    return rval;
};

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs_mtx) {
    if (!exact_size(*this, rhs_mtx)) 
        throw Matrix_Error("[operator(+=/-=)]", "Must have the same size.");

    for (size_t i = 0; i < this->size(); i++)
        data[i] = data[i] + rhs_mtx.data[i];

    return *this;
};

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const T& rhs_type) {
    for (size_t i = 0; i < this->size(); i++)
        data[i] = data[i] + rhs_type;

    return *this;
};

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& rhs_mtx) const {
    return *this + rhs_mtx * -1;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-(const T& rhs_type) {
    *this += rhs_type * -1;
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& rhs_mtx) {
    *this += rhs_mtx * -1;
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const T& rhs_type) {
    *this += rhs_type * -1;
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs_mtx) const {
    if (m != rhs_mtx.n) 
        throw Matrix_Error("[operator(*/*=)]", "Columns of this and rows of other must be equal.");

    Matrix<T> rval(n, rhs_mtx.m);
    for (size_t i = 0; i < n; i++)
        for (size_t k = 0; k < m; k++)
            for (size_t j = 0; j < rhs_mtx.m; j++)
                rval(i, j) += this->operator()(i, k) * rhs_mtx(k, j);
                
    return rval;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const T& rhs_type) const {
    Matrix<T> rval = *this;
    rval *= rhs_type;
    return rval;
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& rhs_mtx) {
    Matrix<T> tmp = this * rhs_mtx;
    this = tmp;
    return *this; 
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const T& rhs_type) {
    for (size_t i = 0; i < this->size(); i++) {
        data[i] = data[i] * rhs_type;
    }
    return *this;
}

template<typename T>
bool Matrix<T>::operator==(const Matrix<T>& other) const {
        if (this == &other) return true;
        if (n != other.n || m != other.m) return false;
        if (n == 0 && m == 0) return true;

        for (size_t i = 0; i < this->size(); i++)
            if (!type_numeric_eq<T>(data[i], other.data[i])) return false;

        return true;
    }
}

#endif

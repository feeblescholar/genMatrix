/**
 * @file matrix_binaryop.ipp
 * @author Kovács Botond
 * @brief Mátrixok közötti vagy balértéken végzett bináris operátorok definíciói.
 */
#ifndef MATRIX_BINARYOP_I
#define MATRIX_BINARYOP_I

#include "matrix.hpp"
#include "typechecks.hpp"

namespace genMatrix {

template<typename T>
template<typename S>
decltype(auto) Matrix<T>::operator+(const Matrix<S>& rhs_mtx) const {
    if constexpr (has_add_v<T, S>) {
        using ReturnType = decltype(T(0) + S(0));

        Matrix<ReturnType> rval = *this;
        Matrix<ReturnType> rhs_mtx_c = rhs_mtx;

        rval += rhs_mtx_c;
        return rval;
    }
}

template<typename T>
template<typename S>
decltype(auto) Matrix<T>::operator+(const S& rhs_type) const {
    if constexpr (has_add_v<T, S>) {
        using ReturnType = decltype(T(0) + S(0));

        Matrix<ReturnType> rval = *this;
        ReturnType rhs_type_c = rhs_type;

        rval += rhs_type_c;
        return rval;
    }
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
template<typename S>
decltype(auto) Matrix<T>::operator-(const Matrix<S>& rhs_mtx) const {
    if constexpr (has_add_v<T, S> && has_mul_v<S, int>) {
        return *this + rhs_mtx * -1;
    }
}

template<typename T>
template<typename S>
decltype(auto) Matrix<T>::operator-(const S& rhs_type) const {
    if constexpr (has_add_v<T, S> && has_mul_v<S, int>) {
        using ReturnType = decltype(T(0) + S(0));

        Matrix<ReturnType> rval = *this;
        ReturnType rhs_type_c = rhs_type;
        rval -= rhs_type_c;

        return rval;
    }
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
template<typename S>
decltype(auto) Matrix<T>::operator*(const Matrix<S>& rhs_mtx) const {
    if constexpr (has_mul_v<T, S>) {
        if (m != rhs_mtx.getRows()) 
            throw Matrix_Error("[operator(*/*=)]", "this.m == other.n");

    using ReturnType = decltype(T(0) * S(0));
    Matrix<ReturnType> rval(n, rhs_mtx.getCols());

    for (size_t i = 0; i < n; i++)
        for (size_t k = 0; k < m; k++)
            for (size_t j = 0; j < rhs_mtx.getCols(); j++)
                rval(i, j) += this->operator()(i, k) * rhs_mtx(k, j);
                
    return rval;
    }
}

template<typename T>
template<typename S>
decltype(auto) Matrix<T>::operator*(const S& rhs_type) const {
    if constexpr (has_mul_v<T, S>) {
        using ReturnType = decltype(T(0) * S(0));

        Matrix<ReturnType> rval = *this;
        ReturnType rhs_type_c = rhs_type;

        rval *= rhs_type_c;
        return rval;
    }
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& rhs_mtx) {
    this = this * rhs_mtx;
    return *this; 
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const T& rhs_type) {
    for (size_t i = 0; i < this->size(); i++)
        data[i] = data[i] * rhs_type;
    return *this;
}

template<typename T>
template<typename S>
bool Matrix<T>::operator==(const Matrix<S>& other) const {
        if (!std::is_same_v<T, S>)
            return false;

        if (this == &other) 
            return true;

        if (n != other.n || m != other.m) 
            return false;

        if (n == 0 && m == 0) 
            return true;

        for (size_t i = 0; i < this->size(); i++)
            if (!type_numeric_eq<T>(data[i], other.data[i])) return false;

        return true;
    }
}

#endif

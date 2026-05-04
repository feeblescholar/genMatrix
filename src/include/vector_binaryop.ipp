/**
 * @file vector_binaryop.ipp
 * @author Kovács Botond
 * @brief Vektorokon közötti műveletek implementációi.
 */
#ifndef VECTOR_BINARYOP_I
#define VECTOR_BINARYOP_I

#include "vector.hpp"

namespace genMatrix {

template<typename T>
template<typename S>
decltype(auto) Vector<T>::operator+(const Vector<S>& rhs_vec) const {
    if constexpr (has_add_v<T, S>) {
        using ReturnType = decltype(T(0) + S(0));

        Vector<ReturnType> rval = *this;
        Vector<ReturnType> tmp = rhs_vec;

        rval += tmp;
        return rval;
    }
    else
        throw Matrix_Error("[operator+]", "Addition is undefined.");
}

template<typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& rhs_vec) {
    if constexpr (has_add_v<T, T>) {
        if (_size != rhs_vec._size)
            throw Vector_Error("[operator+=]", "Must be the same size.");
        
        for (size_t i = 0; i < _size; i++)
            data[i] += rhs_vec[i];
    }
    else
        throw Vector_Error("[operator+=]", "Addition is undefined.");

    return *this;
}

template<typename T>
template<typename S>
decltype(auto) Vector<T>::operator-(const Vector<S>& rhs_vec) const {
    if constexpr (has_add_v<T, S> && has_mul_v<S, int>) {
        using ReturnType = decltype(T(0) + S(0));
        
        Matrix<ReturnType> rval = *this;
        Matrix<ReturnType> tmp = rhs_vec;

        rval += rhs_vec * -1;
        return rval;
    }
    else
        throw Vector_Error("[operator-]", "Required operators are undefined.");
}

template<typename T>
Vector<T>& Vector<T>::operator-=(const Vector<T>& rhs_vec) {
    if constexpr (has_add_v<T, T> && has_mul_v<T, int>)
        *this += rhs_vec * -1;
    else
        throw Vector_Error("[operator-=]", "Required operators are undefined.");
    
    return *this;
}

template<typename T>
template<typename S>
decltype(auto) Vector<T>::operator*(const Vector<S>& rhs_vec) const {
    if constexpr (has_add_v<T, S> && has_mul_v<T, S>) {
        if (_size != rhs_vec._size())
            throw Vector_Error("[operator*]", "Must be the same size.");

        using ReturnType = decltype(T(0) * S(0));
        ReturnType rval = 0;

        for (size_t i = 0; i < _size; i++)
            rval += data[i] * rhs_vec[i];

        return rval;
    }
    else
        throw Vector_Error("[operator*]", "Required operators are undefined.");
}

template<typename T>
template<typename S>
decltype(auto) Vector<T>::operator*(const S& rhs_type) const {
    if constexpr (has_mul_v<T, S>) {
        using ReturnType = decltype(T(0) * S(0));

        Vector<ReturnType> rval = *this;
        ReturnType rhs_type_c = rhs_type;

        for (size_t i = 0; i < _size; i++) {
            rval[i] *= rhs_type;
        }

        return rval;
    }
    else
        throw Vector_Error("[operator*]", "Multiplication is undefined.");
}

template<typename T>
template<typename S>
bool Vector<T>::operator==(const Vector<S>& rhs_vec) const {
    if (!std::is_same_v<T, S>)
        return false;

    if (_size != rhs_vec._size())
        return false;
    
    for (size_t i = 0; i < _size; i++) 
        if (data[i] != rhs_vec[i])
            return false;
    
    return true;
}
}

#endif
/**
 * @file matrix.ipp
 * @author Kovács Botond
 * @brief Generikus mátrix osztály implementáció.
 * @note A függvények dokumentációja a matrix.hpp fájban található.
 */
#ifndef MATRIX_I
#define MATRIX_I

#include <utility>

#include "typechecks.hpp"
#include "custom_type_traits.hpp"

namespace genMatrix {

template<typename T>
Matrix<T>::Matrix(size_t _n, size_t _m) 
    : data(nullptr), 
      n(_n), 
      m(_m), 
      dataLocation(FixedStack) 
{
    if (!n && !m) {
        dataLocation = DynamicHeap;
        return;
    }

    /** Megvizsgáljuk belefér-e a kis bufferbe. (SBO) */
    if (n <= 5 && m <= 5)  {
        data = staticBuf;
        for (size_t i = 0; i < this->size(); i++) 
            data[i] = T();
        return;
    }

    /** forceoljuk a default constructort elemi típusokra is */
    data = new T[this->size()](); 
    dataLocation = FixedHeap;
}

template<typename T>
Matrix<T>::Matrix(const Matrix& other) : data(nullptr), n(0), m(0) {
    *this = other;
}

template<typename T>
template<typename S> 
Matrix<T>::Matrix(const Matrix<S>& other) : data(nullptr), n(0), m(0) {
    *this = other;
}

template<typename T>
Matrix<T>::Matrix(Matrix&& other) : data(nullptr), n(0), m(0) {
    if (other.dataLocation != FixedStack)
        data = std::exchange(other.data, nullptr);
    else {
        data = staticBuf;
        for (size_t i = 0; i < other.size(); i++)
            data[i] = other.data[i];
    }
    
    n = std::exchange(other.n, 0);
    m = std::exchange(other.m, 0);
    dataLocation = other.dataLocation;
}

template<typename T>
Matrix<T>::~Matrix() {
    if (dataLocation != FixedStack)
        delete[] data;
    data = nullptr; /* double delete ellen */
    n = m = 0;
};

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
    if (this != &other)
        this->operator=<T>(other);
        
    return *this;
}

template<typename T> 
template<typename S>
Matrix<T>& Matrix<T>::operator=(const Matrix<S>& other) {
    if (dataLocation != FixedStack) 
        delete[] data;

    dataLocation = other.getDataLocation();
    n = other.getRows();
    m = other.getCols();

    if (dataLocation == FixedStack) 
        data = staticBuf;
    else 
        data = new T[this->size()]();

    for (size_t i = 0; i < this->getRows(); i++)
        for (size_t j = 0; j < this->getCols(); j++)
            this->operator()(i,j) = static_cast<T>(other(i,j));

    return *this;
}


template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) {
    if (dataLocation != FixedStack)
        delete[] data;

    if (other.dataLocation != FixedStack)
        data = std::exchange(other.data, nullptr);
    else {
        data = staticBuf;
        for (size_t i = 0; i < other.size(); i++)
            data[i] = other.data[i];
    }
    
    n = std::exchange(other.n, 0);
    m = std::exchange(other.m, 0);
    dataLocation = other.dataLocation;

    return *this;
}

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
        if (!utils::eq<T>(data[i], other.data[i])) return false;

    return true;
}

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

template<typename T>
void Matrix<T>::resize(size_t _n, size_t _m) {
    if (dataLocation != DynamicHeap)
        throw Matrix_Error("[resize]", "Not allowed on non-dynamic matrices.");
    
    if (_n == 0 || _m == 0) {
        delete[] data;
        data = nullptr; /** Ha esetleg a destruktor ráhívná a deletet. */
        return;
    }

    T* ndata = new T[_n * _m]();

    /** Annyit másolunk, amennyit lehet */
    size_t copysize = std::min(this->size(), _n * _m);

    for (size_t i = 0; i < copysize; i++) {
        ndata[i] = data[i];
    }

    if (data) 
        delete[] data;
    
    data = ndata;
    n = _n;
    m = _m;
}

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
    else
        throw Matrix_Error("[operator+]", "Addition is not defined.");
}

template<typename T>
template<typename S>
decltype(auto) Matrix<T>::operator+(const S& rhs_type) const {
    if constexpr (has_add_v<T, S>) {
        using ReturnType = decltype(T(0) + S(0));

        Matrix<ReturnType> rval = *this;
        ReturnType rhs_type_c = static_cast<ReturnType>(rhs_type);

        rval += rhs_type_c;
        return rval;
    }
    else
        throw Matrix_Error("[operator+]", "Addition is not defined.");
};

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs_mtx) {
    if constexpr (has_add_v<T, T>) {
        if (!exact_size(*this, rhs_mtx)) 
            throw Matrix_Error("[operator(+=/-=)]", "Must have the same size.");

        for (size_t i = 0; i < this->size(); i++)
            data[i] = data[i] + rhs_mtx.data[i];
    }
    else
        throw Matrix_Error("[operator+=]", "Addition is not defined.");

    return *this;
};

template<typename T>
template<typename S>
Matrix<T>& Matrix<T>::operator+=(const S& rhs_type) {
    if constexpr (has_add_v<T, T>) {
        for (size_t i = 0; i < this->size(); i++)
            data[i] = data[i] + static_cast<T>(rhs_type);
    }
    else
        throw Matrix_Error("[operator+=]", "Addition is not defined.");

    return *this;
};

template<typename T>
template<typename S>
decltype(auto) Matrix<T>::operator-(const Matrix<S>& rhs_mtx) const {
    if constexpr (has_add_v<T, S> && has_mul_v<S, int>)
        return *this + rhs_mtx * -1;
    else
        throw Matrix_Error("[operator-]", "Required operators are undefined.");
}

template<typename T>
template<typename S>
decltype(auto) Matrix<T>::operator-(const S& rhs_type) const {
    if constexpr (has_add_v<T, S> && has_mul_v<S, int>) {
        using ReturnType = decltype(T(0) + S(0));

        Matrix<ReturnType> rval = *this;
        ReturnType rhs_type_c = static_cast<ReturnType>(rhs_type);
        rval -= rhs_type_c;

        return rval;
    }
    else
        throw Matrix_Error("[operator-]", "Required operators are undefined.");
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& rhs_mtx) {
    if constexpr (has_add_v<T, T> && has_mul_v<T, int>)
        *this += rhs_mtx * -1;
    else
        throw Matrix_Error("[operator-=]", "Required operators are undefined.");
    
    return *this;
}

template<typename T>
template<typename S>
Matrix<T>& Matrix<T>::operator-=(const S& rhs_type) {
    if constexpr (has_add_v<T, T> && has_mul_v<T, int>)
        *this += static_cast<T>(rhs_type) * -1;
    else
        throw Matrix_Error("[operator-=]", "Required operators are undefined.");
    
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
    else
        throw Matrix_Error("[operator*]", "Multiplication is undefined.");
}

template<typename T>
template<typename S>
decltype(auto) Matrix<T>::operator*(const S& rhs_type) const {
    if constexpr (has_mul_v<T, S>) {
        using ReturnType = decltype(T(0) * S(0));

        Matrix<ReturnType> rval = *this;
        ReturnType rhs_type_c = (ReturnType)rhs_type;

        rval *= rhs_type_c;
        return rval;
    }
    else
        throw Matrix_Error("[operator*]", "Multiplication is undefined.");
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& rhs_mtx) {
    if constexpr (has_mul_v<T, T>) {
        Matrix<T> res = *this * rhs_mtx;
        *this = std::move(res);
    }
    else
        throw Matrix_Error("[operator*=]", "Multiplication is undefined.");

    return *this; 
}

template<typename T>
template<typename S>
Matrix<T>& Matrix<T>::operator*=(const S& rhs_type) {
    if constexpr (has_mul_v<T, T>) {
        for (size_t i = 0; i < this->size(); i++)
            data[i] = data[i] * static_cast<T>(rhs_type);
    }
    else
        throw Matrix_Error("[operator*=]", "Multiplication is undefined.");
    return *this;
}

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


template<typename T>
decltype(auto) Matrix<T>::inverse() const {
    if (n != m)
        throw Matrix_Error("[inv]", "Matrix must be square.");

    using ReturnType = decltype(T(0) + double(0));
   
    /** az inverz mátrix kezdeti állapota egy n*n-es egységmátrix */
    Matrix<ReturnType> ret(n, n);
    for (size_t i = 0; i < n; i++) 
        ret(i, i) = T(1.0);

    Matrix<ReturnType> tmp = *this;

    for (size_t i = 0; i < n; i++) {
        size_t pivotR = i;

        for (size_t j = i; j < n; j++) {
            if (utils::abs(tmp(j, i)) > utils::abs(tmp(pivotR, i))) {
                pivotR = j;
            }
        }

        if (pivotR != i) {
            tmp.swapRow(pivotR, i);
            ret.swapRow(pivotR, i);
        }

        if (utils::eq<ReturnType>(utils::abs(tmp(i, i)), T(0.0)))
            throw Matrix_Error("[inv]", "Singular matrix.");

        ReturnType pivotVal = tmp(i, i);
        for (size_t j = 0; j < n; j++) {
            tmp(i, j) /= pivotVal;
            ret(i, j) /= pivotVal;
        }

        for (size_t j = 0; j < n; j++) {
            if (j != i) {
                ReturnType factor = tmp(j, i);
                for (size_t k = 0; k < n; k++) {
                    /** fma: (ld. determinant.hpp) */
                    tmp(j, k) = utils::fma(-factor, tmp(i, k), tmp(j, k));
                    ret(j, k) = utils::fma(-factor, ret(i, k), ret(j, k));
                }
            }     
        }
    }

    return ret;
}

}

#endif
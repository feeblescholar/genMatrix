/**
 * @file matrix_constructors.ipp
 * @author Kovács Botond
 * @brief Mátrix konstruktor, generikus copy konstruktor, move konstruktor és destruktor
 *        definíciók.
 */
#ifndef MATRIX_CONSTRUCTORS_I
#define MATRIX_CONSTRUCTORS_I

#include "matrix.hpp"

#include <utility>

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
}

#endif

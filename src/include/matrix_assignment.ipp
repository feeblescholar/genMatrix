/**
 * @file matrix_assignment.ipp
 * @author Kovács Botond
 * @brief Mátrix értékadó operátorok implementációi.
 */
#ifndef MATRIX_ASSIGNMENT_I
#define MATRIX_ASSIGNMENT_I

#include "matrix.hpp"

namespace genMatrix {
    
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
}

#endif

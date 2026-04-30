/**
 * @file matrix_sizemanip.ipp
 * @author Kovács Botond
 * @brief Mátrix újraméretezés implementáció.
 */
#ifndef MATRIX_SIZEMANIP_I
#define MATRIX_SIZEMANIP_I

#include "matrix.hpp"

namespace genMatrix {

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
}

#endif

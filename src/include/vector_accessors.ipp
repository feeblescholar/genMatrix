/**
 * @file vector_accessors.ipp
 * @author Kovács Botond
 * @brief Vektor indexeléséhez használt tagfüggvények definíciói.
 */
#ifndef VECTOR_ACCESSSORS_I
#define VECTOR_ACCESSSORS_I

#include "vector.hpp"

namespace genMatrix {

template<typename T>
const T& Vector<T>::operator[](const size_t idx) const {
    if (idx >= _size)
        throw Vector_Error("operator[]", "Out of index.");
    
    return data[idx];
}

template<typename T>
T& Vector<T>::operator[](const size_t idx) {
    if (idx >= _size)
        throw Vector_Error("[operator[]]", "Out of index.");
    
    return data[idx];
}
}
    
#endif

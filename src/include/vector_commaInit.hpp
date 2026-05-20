/**
 * @file vector_commainit.hpp
 * @author Kovács Botond
 * @brief A vektorok feltöltéséhez használt segédosztály definíciója, valamint a 
 *        hozzátartozó vektor operátoré.
 */
#ifndef VECTOR_COMMAINIT_H
#define VECTOR_COMMAINIT_H

#include "vector.hpp"

namespace genMatrix {
/**
* @class CommaInit
* @brief Segédosztály az Eigen stílusú vektorfeltöltéshez.
*/
template<typename T> 
class Vector<T>::CommaInit {
    Vector<T>& vec;  /** A cél vektor referenciája. */

public:
    /**
    * @brief Berakja a vektorba az első elemmet.
    * @param _vec A feltöltendő vektor referenciája.
    * @param init Az első érték.
    */
    template<typename S>
    CommaInit(Vector<T>& _vec, const S& init) : vec(_vec) {
        vec.push_back(static_cast<T>(init));
    }

    /**
    * @brief A vec referencián elérhető vektort tölti fel a következő elemmel.
    * @param val A következő üres helyre szánt érték.
    * @return Önmaga referenciája, így lehet láncban hívni.
    */
    template<typename S>
    CommaInit& operator,(const S& rhs_val) {
        vec.push_back(static_cast<T>(rhs_val));
        return *this;
    }

    ~CommaInit() {}
};

template<typename T>
template<typename S>
typename Vector<T>::CommaInit Vector<T>::operator<<(const S& val) {
    return CommaInit(*this, val);
}
}

#endif
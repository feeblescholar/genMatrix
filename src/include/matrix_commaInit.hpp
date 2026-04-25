/**
 * @file matrix_commainit.hpp
 * @author Kovács Botond
 * @brief A statikus mátrixok feltöltéséhez használt segédosztály definiciója,
 *        valamint a hozzátartozó mátrix operátoré.
 */
#ifndef COMMAINIT_H
#define COMMAINIT_H

#include "matrix.hpp"

namespace genMatrix {
/**
* @class CommaInit
* @brief Segédosztály az Eigen stílusú mátrixfeltöltéshez.
* @details Egy segédosztály segítségével, a mátrixot fel lehet tölteni sorfolytonosan.
*          Ez időt spórol ha fel kell tölteni egy fix méretű mátrixot. Értékadás 
*          során pontosan annyi értéket kell megadni, amekkora a mátrix mérete, 
*          ezért csak ismert, fix méret esetén alkalmazható.
*/
template<typename T> 
class Matrix<T>::CommaInit {
    Matrix<T>& mtx;  /** A cél mátrix referenciája. */
    size_t nextidx;  /** A következő üres index mtx.data-ban. */

public:
    /**
    * @brief Berakja a mátrixba az első elemmet.
    * @param _mtx A feltöltendő mátrix referenciája.
    * @param init Az első érték.
    */
    CommaInit(Matrix<T>& _mtx, const T& init) : mtx(_mtx), nextidx(0) {
        mtx.data[nextidx++] = init;
    }

    /**
    * @brief Az mtx referencián elérhető mátrixot tölti fel a következő elemmel.
    * @param val A következő üres helyre szánt érték.
    * @return Önmaga referenciája, így lehet láncban hívni.
    * @throws Matrix_Error hiba, ha nem lehet az értéket a mátrixba tenni.
    */
    CommaInit& operator,(const T& rhs_val) {
        if (nextidx == mtx.size()) 
            throw Matrix_Error("[CommaInit]", "Too many parameters.", true);

        mtx.data[nextidx++] = rhs_val;
        return *this;
    }

    /**
     * @throws Matrix_Error hiba, ha nincs az összes elem megadva.
     * @note A destruktor C++11 óta implicit noexcept. Ezt itt explicit letiltottuk.
     */
    ~CommaInit() noexcept(false) {
        if (nextidx != mtx.size()) 
            throw Matrix_Error("[CommaInit]", "Not enough parameters.", true);
    }
};

template<typename T>
typename Matrix<T>::CommaInit Matrix<T>::operator<<(const T& val) {
    if (dataLocation == DynamicHeap) 
        throw Matrix_Error("[operator<<]", "Can only be used on static matrices.");

    return CommaInit(*this, val);
}
}

#endif

#ifndef COMMAINIT_H
#define COMMAINIT_H

#include "matrix.hpp"

namespace genMatrix {
    /**
    * @struct CommaInit
    * @brief Segédosztály az Eigen stílusú mátrixfeltöltéshez.
    * @details Egy segédosztály segítségével, a mátrixot fel lehet tölteni sorfolytonosan ((0,0) ... (0,m) ... (1, 0) ...).
    *          Ez időt spórol ha fel kell tölteni egy fix méretű mátrixot. Értékadás során pontosan annyi értéket kell megadni, 
    *          amekkora a mátrix mérete, ezért csak ismert, fix méret esetén alkalmazható.
    * @note Ez a segédosztály az egyszerűsége miatt teljesen public.
    */
    template<typename T> class Matrix<T>::CommaInit {
    public:
        Matrix<T>& mtx;  /** A cél mátrix referenciája. */
        size_t nextidx;  /** A következő üres index mtx.data-ban. */

        /**
        * Berakja a mátrixba az első elemmet.
        * @param _mtx A feltöltendő mátrix referenciája.
        * @param init Az első érték.
        */
        CommaInit(Matrix<T>& _mtx, const T& init) : mtx(_mtx), nextidx(0) {
            mtx.data[nextidx++] = init;
        }

        /**
        * Az mtx referencián elérhető mátrixot tölti fel a következő elemmel.
        * @param val A következő üres helyre szánt érték.
        * @return Önmaga referenciája, így lehet láncban hívni.
        */
        CommaInit& operator,(const T& rhs_val) {
            if (nextidx == mtx.size()) 
                throw Matrix_Error("[CommaInit]", "Too many parameters.", true);

            mtx.data[nextidx++] = rhs_val;
            return *this;
        }

        ~CommaInit() noexcept(false) {
            if (nextidx != mtx.size()) 
                throw Matrix_Error("[CommaInit]", "Not enough parameters.", true);
        }
    };
}

#endif
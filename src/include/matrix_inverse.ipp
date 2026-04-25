#ifndef MATRIX_INVERSE_H
#define MATRIX_INVERSE_H

#include "matrix.hpp"
#include "matrix_error.hpp"
#include "type_func.hpp"

namespace genMatrix {
/**
 * @details Inverz mátrix kiszámítása a Gauss-Jordan eliminációval. Az inverz 
 *          mátrix kiszámításához eliminálni kell az (M|E) mátrixot, ahol E az
 *          egységmátrix és M az eredeti. Az elmináció végén M redukált lépcsős
 *          alakba kerül és E lesz az inverz mátrix. A determinánshoz hasonlóan
 *          a pontosságot a mátrix elemei és kondíciója is befolyásolják.
 */
template<typename T> 
auto Matrix<T>::inverse() const {
    if (n != m)
        throw Matrix_Error("[inv]", "Matrix must be square.");

    /** type promotion szükséges, az inverz szinte sosem egész */
    using ReturnType = decltype(T(0) + double(0));

    /** az inverz mátrix kezdeti állapota egy n*n-es egységmátrix */
    Matrix<ReturnType> inv(n, n);
    for (size_t i = 0; i < n; i++) inv(i, i) = 1.0;

    Matrix<ReturnType> tmp = *this;

    for (size_t i = 0; i < tmp.getRows(); i++) {
        size_t pivotR = i;;

        for (size_t j = i; j < tmp.getRows(); j++) {
            if (std::abs(tmp(j, i)) > std::abs(tmp(pivotR, i))) {
                pivotR = j;
            }
        }

        if (pivotR != i) {
            tmp.swapRow(pivotR, i);
            inv.swapRow(pivotR, i);
        }

        if (type_numeric_eq<T>(std::abs(tmp(i, i)), T(0.0)))
            throw Matrix_Error("[inv]", "Singular matrix.");

        ReturnType pivotVal = tmp(i, i);
        for (size_t j = 0; j < tmp.getRows(); j++) {
            tmp(i, j) /= pivotVal;
            inv(i, j) /= pivotVal;
        }

        for (size_t j = 0; j < tmp.getRows(); j++) {
            if (j != i) {
                long double factor = tmp(j, i);
                for (size_t k = 0; k < tmp.getRows(); k++) {
                    /** fma: (ld. determináns.hpp) */
                    tmp(j, k) = std::fma(-factor, tmp(i, k), tmp(j, k));
                    inv(j, k) = std::fma(-factor, inv(i, k), inv(j, k));
                }
            }     
        }
    }

    return inv;
}
}

#endif

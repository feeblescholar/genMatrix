#ifndef GENMATRIX_DET
#define GENMATRIX_DET

#include "genMatrix.hpp"
#include "matrix_error.hpp"

namespace genMatrix {
    /**
    * @brief Kiszámítja a mátrix determinánsát.
    * @details Kiszámítja a mátrix determinánsát Gauss-eliminációval. A megoldás pontosságát a mátrix
    *          mérete és a tartalma is befolyásolja. Mivel egész mátrixok determinánsánál is előfordulhat
    *          nem egész érték (nem a végeredmény során), ezért minden típus automatikusan long double-re
    *          van castolva.
    * @return A mátrix determinánsa a mátrix típusának megfelelően.
    * @throw Matrix_Error kivétel, ha nem létezik.
    * @warning A mátrix mérete (\c n) erősen befolyásolja a futási időt.
    */
    template<typename T> T det(const Matrix<T>& mtx) {
        if (mtx.getRows() != mtx.getCols()) throw Matrix_Error("[det]", "Must be a square matrix with size of (n x n)");
        
        /** Innentől elég csak n-t vagy m-t vizsgálni (mert négyzetes) */
        if (mtx.getRows() == 0) throw Matrix_Error("[det]", "Matrix is empty.");

        /* 1x1-es mátrix determinánsa maga az elem */
        if (mtx.getRows() == 1) return mtx(0, 0);

        /* 2x2-es mátrix determinánsa ad - bc */
        if (mtx.getRows() == 2) return mtx(0, 0) * mtx(1, 1) - mtx(0, 1) * mtx(1, 0);

        Matrix<long double> tmp = mtx;
        long double det(1.0);
        int sign = 1;

        for (size_t i = 0; i < tmp.getRows(); i++) {
            /** Tervvel ellentétben full pivoting lesz, az pontosabb */

            size_t pivotR = i;
            size_t pivotC = i;
            long double max = -1;

            for (size_t j = i; j < tmp.getRows(); j++) {
                for (size_t k = i; k < tmp.getCols(); k++) {
                    if (std::abs(tmp(j, k)) > max) {
                        max = std::abs(tmp(j, k));
                        pivotC = i;
                        pivotR = j;
                    }
                }
            }

            if (max < std::numeric_limits<long double>::epsilon()) return T(0);

            if (pivotR != i) {
                tmp.swapRow(pivotR, i);
                sign *= -1.0;
            }

            if (pivotC != i) {
                tmp.swapCol(pivotC, i);
                sign *= -1.0;
            }

            for (size_t j = i + 1; j < tmp.getRows(); j++) {
                long double div = tmp(j, i) / tmp(i, i);

                for (size_t k = i + 1; k < tmp.getRows(); k++) {
                    tmp(j, k) = std::fma(-div, tmp(i, k), tmp(j, k));
                }
            }
        }

        for (size_t i = 0; i < tmp.getRows(); i++) {
            det *= tmp(i, i);
        }

        return static_cast<T>(det * sign);
    }


}


#endif
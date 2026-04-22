#ifndef GENMATRIX_DET
#define GENMATRIX_DET

#include "genMatrix.hpp"
#include "matrix_error.hpp"

namespace genMatrix {
    /**
    * @brief Kiszámítja a mátrix determinánsát.
    * @details Kiszámítja a mátrix determinánsát Gauss-eliminációval. Full pivotingot alkalmazunk, így nagy számokkal
    *          osztunk le, megtartva (a lehető legjobban) a pontosságot. A megoldás pontosságát a mátrix mérete és a 
    *          tárolt értékei is befolyásolják, rosszul kondícionált ("majdnem szinguláris") mátrix esetében az eredmény
    *          jelentősen eltérhet a valóságtól (1 - 2 tizedesjegyel az epsilon felett). Kondícionált mátrixok esetében ez
    *          nem okoz nagy eltérést. A pontosság további javítása érdekében FMA-t (Fused Multiply-Add) is használunk, ami
    *          1 órajel alatt kiszámítja [a * b + c]-t és csak egyszer kerekíti az értéket.
    * @return A mátrix determinánsa a mátrix típusának megfelelően.
    * @throw Matrix_Error kivétel, ha nem létezik.
    * @warning A mátrix mérete (\c n) erősen befolyásolja a futási időt.
    */
    template<typename T> typename std::enable_if_t<std::is_floating_point_v<T>, T> det(const Matrix<T>& mtx) {
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
                    /** fma: nagyon fullos (ld. fent) */
                    tmp(j, k) = std::fma(-div, tmp(i, k), tmp(j, k));
                }
            }
        }

        for (size_t i = 0; i < tmp.getRows(); i++) {
            det *= tmp(i, i);
        }

        return static_cast<T>(det * sign);
    }

    /**
    * @brief Kiszámítja a mátrix determinánsát.
    * @details Kiszámítja a mátrix determinánsát a Bareiss-alogritmussal. Az algoritmus kifejezetten hasznos, mert
    *          kizárólag egészekkel dolgozik, így nem zavarnak be a lebegőpontos aritmetikai hibák. A megoldás pontos, az
    *          előbbiek miatt 0-ás epsilonnal tesztelhető.
    * @return A mátrix determinánsa a mátrix típusának megfelelően.
    * @throw Matrix_Error kivétel, ha nem létezik.
    * @warning A mátrix mérete erősen befolyásolja a futási időt (O(n^3)). Nagy értékek esetén előfordulhat integer overflow,
    *          ennek elkerülése érdekében minden típust int128-re emelünk a számítás idejére.
    */
    template<typename T> typename std::enable_if_t<std::is_integral_v<T>, T> det(const Matrix<T>& mtx) {
        if (mtx.getRows() != mtx.getCols()) throw Matrix_Error("[det]", "Must be a square matrix with size of (n x n)");
        
        /** Innentől elég csak n-t vagy m-t vizsgálni (mert négyzetes) */
        if (mtx.getRows() == 0) throw Matrix_Error("[det]", "Matrix is empty.");

        /* 1x1-es mátrix determinánsa maga az elem */
        if (mtx.getRows() == 1) return mtx(0, 0);

        /* 2x2-es mátrix determinánsa ad - bc */
        if (mtx.getRows() == 2) return mtx(0, 0) * mtx(1, 1) - mtx(0, 1) * mtx(1, 0);

        Matrix<__int128_t> tmp = mtx;
        int sign = 1;
        __int128_t pivotPrev = 1;
        
        for (size_t i = 0; i < tmp.getRows(); i++) {
            
            size_t pivotR = i;
            
            while (pivotR < tmp.getRows() && tmp(pivotR, i) == 0)
                pivotR++;

            if (pivotR == tmp.getRows()) return 0;

            if (pivotR != i) {
                tmp.swapRow(pivotR, i);
                sign = sign * -1;
            }

            for (size_t j = i + 1; j < tmp.getRows(); j++) {
                for (size_t k = i + 1; k < tmp.getRows(); k++) {
                    tmp(j, k) = (tmp(i, i) * tmp(j, k) - tmp(j, i) * tmp(i, k)) / pivotPrev;
                }
            }

            pivotPrev = tmp(i, i);
        }

        return sign * tmp(tmp.getRows() - 1, tmp.getRows() - 1);
    }
}


#endif
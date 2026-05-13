/**
 * @file norms.hpp
 * @author Kovács Botond
 * @brief Mátrixok normáinak kiszámítása.
 */
#ifndef NORMS_H
#define NORMS_H

#include "matrix.hpp"

namespace genMatrix {

/**
 * @param mtx - A normált mátrix referenciája.
 * @return A mátrix 1-normája.
 */
template<typename T>
auto norm_1(const Matrix<T>& mtx) {
    /** itt ez megoldja azon problémát, hogy a decltype előléptet */
    using ReturnType = decltype(utils::abs(T(0)));

    ReturnType norm = ReturnType(0);

    for (size_t i = 0; i < mtx.getCols(); i++) {
        ReturnType sum = ReturnType(0);

        for (size_t j = 0; j < mtx.getRows(); j++) {
            sum += utils::abs(mtx(j, i));
        }

        if (sum > norm) norm = sum;
    }

    return norm;
}

/**
 * @param mtx - A normált mátrix referenciája.
 * @return A mátrix végtelen (maximum) normája.
 */
template<typename T>
auto norm_inf(const Matrix<T>& mtx) {
    /** itt ez megoldja azon problémát, hogy a decltype előléptet */
    using ReturnType = decltype(utils::abs(T(0)));

    ReturnType norm = ReturnType(0);

    for (size_t i = 0; i < mtx.getRows(); i++) {
        ReturnType sum = ReturnType(0);

        for (size_t j = 0; j < mtx.getCols(); j++) {
            sum += utils::abs(mtx(i, j));
        }

        if (sum > norm) norm = sum;
    }

    return norm;
}

/**
 * @param mtx - A normált mátrix referenciája.
 * @return A mátrix Frobenius normája.
 * @note Minimum floatra lépteti elő az eredményt.
 */
template<typename T>
auto norm_frobenius(const Matrix<T>& mtx) {
    /** itt ez megoldja azon problémát, hogy a decltype előléptet */
    using ReturnType = decltype(utils::abs(T(0)));

    ReturnType norm = ReturnType(0);

    for (size_t i = 0; i < mtx.getRows(); i++) {
        for (size_t j = 0; j < mtx.getCols(); j++) {
            norm += utils::abs(mtx(i, j)) * utils::abs(mtx(i, j));
        }
    }
    
    return utils::sqrt(norm);
}
}

#endif

#ifndef NORMS_H
#define NORMS_H

#include "matrix.hpp"

namespace genMatrix {

    /**
     * @param mtx - A normált mátrix referenciája.
     * @return A mátrix 1-normája.
     */
    template<typename T> T norm_1(const Matrix<T>& mtx) {
        T norm = T(0);

        for (size_t i = 0; i < mtx.getCols(); i++) {
            T sum = T(0);

            for (size_t j = 0; j < mtx.getRows(); j++) {
                sum += std::abs(mtx(j, i));
            }

            if (sum > norm) norm = sum;
        }

        return norm;
    }

    /**
     * @param mtx - A normált mátrix referenciája.
     * @return A mátrix végtelen (maximum) normája.
     */
    template<typename T> T norm_inf(const Matrix<T>& mtx) {
        T norm = T(0);

        for (size_t i = 0; i < mtx.getRows(); i++) {
            T sum = T(0);

            for (size_t j = 0; j < mtx.getCols(); j++) {
                sum += std::abs(mtx(i, j));
            }

            if (sum > norm) norm = sum;
        }

        return norm;
    }

    /**
     * @param mtx - A normált mátrix referenciája.
     * @return A mátrix Frobenius normája.
     */
    template<typename T> T norm_frobenius(const Matrix<T>& mtx) {
        T norm = T(0);

        for (size_t i = 0; i < mtx.getRows(); i++) {
            for (size_t j = 0; j < mtx.getCols(); j++) {
                norm = std::hypot(norm, mtx(i, j));
            }
        }
        
        return norm;
    }
}

#endif
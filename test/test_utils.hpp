/**
 * @file test_utils.hpp
 * @author Kovács Botond
 * @brief Teszteléshez használt segédfüggvények.
 */
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <gtest/gtest.h>
#include <random>
#include "../src/include/genMatrix.hpp"

/**
 * @brief Összehasonlít két számot az EXPECT_NEAR GoogleTest makróval.
 */
template<typename T>
void CMP_VAL(const T& a, const T& b, const double eps) {
    if constexpr (genMatrix::internal::type_traits::is_hypercomplex2<T>) {
        EXPECT_NEAR(a.real(), b.real(), eps);
        EXPECT_NEAR(a.unreal(), b.unreal(), eps);
    } 
    else
        EXPECT_NEAR(a, b, eps);
}

/**
 * @brief Összehasonlít két mátrixot az EXPECT_NEAR GoogleTest makróval.
 */
template<typename T>
void CMP_MTX(const genMatrix::Matrix<T>& a, const genMatrix::Matrix<T>& b, const double eps) {
    ASSERT_EQ(a.getRows(), b.getRows());
    ASSERT_EQ(a.getCols(), b.getCols());

    for (size_t i = 0; i < a.getRows(); ++i) {
        for (size_t j = 0; j < a.getCols(); ++j) {
            CMP_VAL(a(i, j), b(i, j), eps);
        }
    }
}

template<typename T>
T RNG(int lbound, int ubound) {
    static std::default_random_engine eng(std::random_device{}());

    if constexpr (genMatrix::internal::type_traits::has_no_precision<T>) {
        std::uniform_int_distribution<T> dist(lbound, ubound);
        return T(dist(eng));
    }
    else {
        if constexpr (genMatrix::internal::type_traits::is_hypercomplex2<T>) {
            std::uniform_real_distribution<double> dist(lbound, ubound);
            return T(dist(eng), dist(eng));
        }
        else {
            std::uniform_real_distribution<T> dist(lbound, ubound);
            return T(dist(eng));
        }
    }
}

template<typename T>
genMatrix::Matrix<T> RNG_MTX(size_t n, size_t m, int lbound, int ubound) {
    genMatrix::Matrix<T> mtx(n, m);

    for(size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            mtx(i, j) = RNG<T>(lbound, ubound);

    return mtx;
}


#endif
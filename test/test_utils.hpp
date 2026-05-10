/**
 * @file test_utils.hpp
 * @author Kovács Botond
 * @brief Teszteléshez használt segédfüggvények.
 */
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <gtest/gtest.h>
#include "../src/include/genMatrix.hpp"

/**
 * @brief Összehasonlít két számot az EXPECT_NEAR GoogleTest makróval.
 */
template<typename T>
inline void CMP_VAL(const T& a, const T& b, const double eps) {
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
inline void CMP_MTX(const genMatrix::Matrix<T>& a, const genMatrix::Matrix<T>& b, const double eps) {
    ASSERT_EQ(a.getRows(), b.getRows());
    ASSERT_EQ(a.getCols(), b.getCols());

    for (size_t i = 0; i < a.getRows(); ++i) {
        for (size_t j = 0; j < a.getCols(); ++j) {
            CMP_VAL(a(i, j), b(i, j), eps);
        }
    }
}

#endif
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
void CMP_MTX(const genMatrix::Matrix<T>& a, 
             const genMatrix::Matrix<T>& b, 
             const double eps) 
{
    EXPECT_EQ(a.getRows(), b.getRows());
    EXPECT_EQ(a.getCols(), b.getCols());

    for (size_t i = 0; i < a.getRows(); i++)
        for (size_t j = 0; j < a.getCols(); j++)
            CMP_VAL(a(i, j), b(i, j), eps);

}

/**
 * @brief Összehasonlít két vektort az EXPECT_NEAR GoogleTest makróval.
 */
template<typename T>
void CMP_VEC(const genMatrix::Vector<T>& a, 
             const genMatrix::Vector<T>& b, 
             const double eps) 
{
    EXPECT_EQ(a.size(), b.size());

    for (size_t i = 0; i < a.size(); i++)
        CMP_VAL(a[i], b[i], eps);
}

/**
 * @brief Generál egy random értéket típusnak megfelelően.
 * @param lbound A random érték alsó határa.
 * @param ubound A random érték felső határa.
 * @return A random érték.
 */
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

/**
 * @brief Generál egy random értékekkel teli mátrixot típusnak megfelelően.
 * @param n A mátrix sorainak száma.
 * @param m A mátrix oszlopainak száma.
 * @param lbound A random értékek alsó határa.
 * @param ubound A random értékek felső határa.
 * @return A random mátrix.
 * @note A mátrixnak van move konstruktora, így nagy mátrixok esetében sem okoz
 *       jelentős lassulást.
 */
template<typename T>
genMatrix::Matrix<T> RNG_MTX(size_t n, size_t m, int lbound, int ubound) {
    genMatrix::Matrix<T> mtx(n, m);

    for(size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            mtx(i, j) = RNG<T>(lbound, ubound);

    return mtx;
}


#endif
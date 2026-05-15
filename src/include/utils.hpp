/**
 * @file utils.hpp
 * @author Kovács Botond
 * @brief A genMatrix segédfüggvényeinek implementáció.
 */
#ifndef UTILS_H
#define UTILS_H

#include <limits>
#include <cmath>
#include <type_traits>
#include <algorithm>

#include "custom_type_traits.hpp"

namespace genMatrix {
    class Complex;
}

namespace genMatrix::utils {
/**
 * @brief Egyenlőség vizsgálat egészekre.
 */
template<typename T> 
inline typename std::enable_if_t<std::is_integral_v<T>, bool>
eq(const T& a, const T& b, ...);

/**
 * @brief Megállapítja, hogy két típus epsilon hibával egyenlőnek mondható-e.
 *        Figyelembe veszi a számok nagyságrendjét is, de epsilon alá nem megy.
 * @return Igaz, ha a két szám távolsága kisebb, mint epsilon.
 * @details Ez alapján: 
 *          https://libeigen.gitlab.io/eigen/docs-5.0/classEigen_1_1DenseBase.html#ae8443357b808cd393be1b51974213f9c
 */
template<typename T> 
inline typename std::enable_if_t<std::is_floating_point_v<T>, bool>
eq(const T& a, const T& b, T eps = std::numeric_limits<T>::epsilon() * 1e3);

/**
 * @brief Összehasonlít két 2D-s hiperkomplex számot komponensenként.
 * @return Igaz, ha a komponensenkénti távolság kisebb, mint epsilon.
 */
template<typename T>
inline typename std::enable_if_t<internal::type_traits::is_hypercomplex2<T>, bool>
eq(const T& a, const T& b, const double eps = std::numeric_limits<double>::epsilon() * 1e3);

/** std függvények custom típusra való kiterjesztései */

/** 
 * @param a A szám referenciája.
 * @return A szám abszolútértéke.
 */
template<typename T>
inline typename std::enable_if_t<std::is_arithmetic_v<T>, T> 
abs(const T& a);

/** 
 * @param a A szám referenciája.
 * @return A szám abszolútértéke.
 */
template<typename T>
inline typename std::enable_if_t<internal::type_traits::is_hypercomplex2<T>, double>
abs(const T& a);

/** 
 * @param a A szám referenciája.
 * @return A szám abszolútértéke.
 */
template<typename T>
inline typename std::enable_if_t<std::is_arithmetic_v<T>, decltype(T(0) + float(0))> 
sqrt(const T& a);

/**
 * @note Complex négyzetgyök pozitív ágát adja vissza.
 */
template<typename T>
inline genMatrix::Complex sqrt(const genMatrix::Complex a);

/**
 * @brief Fuse Multiply-Add elemi típusokra.
 * @param a Az egyik szorzótényező referenciája.
 * @param b A másik szorzótényező referenciája.
 * @param c Azon szám referenciája amit a szorzathoz adunk.
 * @return A forrástípus, melynek értéke a * b + c
 */
template<typename T>
inline typename std::enable_if_t<std::is_arithmetic_v<T>, T>
fma(const T& a, const T& b, const T& c);

/**
 * @brief Fuse Multiply-Add komplexre.
 * @param a Az egyik szorzótényező referenciája.
 * @param b A másik szorzótényező referenciája.
 * @param c Azon szám referenciája amit a szorzathoz adunk.
 * @return Egy új komplex, melynek értéke a * b + c
 */
inline Complex 
fma(const Complex& a, const Complex& b, const Complex& c);
}

#include "utils.ipp"

#endif
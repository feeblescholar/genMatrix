#ifndef UTILS_I
#define UTILS_I

#include "complex.hpp"

namespace genMatrix::utils {
/**
 * @brief Egyenlőség vizsgálat egészekre.
 */
template<typename T> 
inline typename std::enable_if_t<std::is_integral_v<T>, bool>
eq(const T& a, const T& b, ...) {
    return a == b;
}

/**
 * @brief Megállapítja, hogy két típus epsilon hibával egyenlőnek mondható-e.
 *        Figyelembe veszi a számok nagyságrendjét is, de epsilon alá nem megy.
 * @return Igaz, ha a két szám távolsága kisebb, mint epsilon.
 * @details Ez alapján: 
 *          https://libeigen.gitlab.io/eigen/docs-5.0/classEigen_1_1DenseBase.html#ae8443357b808cd393be1b51974213f9c
 */
template<typename T> 
inline typename std::enable_if_t<std::is_floating_point_v<T>, bool>
eq(const T& a, const T& b, T eps) {    
    T diff = std::abs(a - b);
    T norm = std::numeric_limits<T>::min();

    if (std::abs(a) < norm && std::abs(b) < norm) {
        return diff < (eps * norm);
    }

    return diff <= eps * std::max(std::abs(a), std::abs(b));
}

/**
 * @brief Összehasonlít két 2D-s hiperkomplex számot komponensenként.
 * @return Igaz, ha a komponensenkénti távolság kisebb, mint epsilon.
 */
template<typename T>
inline typename std::enable_if_t<internal::type_traits::is_hypercomplex2<T>, bool>
eq(const T& a, const T& b, const double eps) {
    return eq<double>(a.real(), b.real(), eps) && eq<double>(a.unreal(), b.unreal(), eps);
}

/** std függvények custom típusra való kiterjesztései */

template<typename T>
inline typename std::enable_if_t<std::is_arithmetic_v<T>, T>
abs(const T& a) {
    return std::abs(a);
}

template<typename T>
inline typename std::enable_if_t<internal::type_traits::is_hypercomplex2<T>, double>
abs(const T& a) {
    return a.abs();
}

template<typename T>
inline typename std::enable_if_t<std::is_arithmetic_v<T>, T>
fma(const T& a, const T& b, const T& c) {
    return std::fma(a, b, c);
}

inline Complex fma(const Complex& a, const Complex& b, const Complex& c) {
    double aR = a.getRe();
    double aI = a.getIm();
    double bR = b.getRe();
    double bI = b.getIm();
    double cR = c.getRe();
    double cI = c.getIm();

    double fmaReal = std::fma(aR, bR, std::fma(-aI, bI, cR));
    double fmaImag = std::fma(aR, bI, std::fma(aI, bR, cI));

    return Complex(fmaReal, fmaImag);
}

}

#endif
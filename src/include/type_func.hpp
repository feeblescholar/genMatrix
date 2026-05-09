#ifndef TYPE_FUNC_H
#define TYPE_FUNC_H

#include <limits>
#include <cmath>
#include <type_traits>
#include <algorithm>

#include "custom_type_traits.hpp"

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
eq(const T& a, const T& b, T eps = std::numeric_limits<T>::epsilon() * 1e3) {    
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
eq (const T& a, const T& b, const double eps = std::numeric_limits<T>::epsilon() * 1e3) {
    return eq<double>(a.real(), b.real(), eps) && eq<double>(a.unreal(), b.unreal(), eps);
}
}

#endif
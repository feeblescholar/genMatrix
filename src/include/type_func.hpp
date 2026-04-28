#ifndef TYPE_FUNC
#define TYPE_FUNC

#include <limits>
#include <cmath>
#include <type_traits>
#include <algorithm>

/**
 * @brief Megállapítja, hogy két típus epsilon hibával egyenlőnek mondható-e.
 *        Figyelembe veszi a számok nagyságrendjét is, de epsilon alá nem megy.
 * @details Ez alapján: 
 *          https://libeigen.gitlab.io/eigen/docs-5.0/classEigen_1_1DenseBase.html#ae8443357b808cd393be1b51974213f9c
 */
template<typename T> 
bool type_numeric_eq(const T& a, const T& b, 
    T eps = std::numeric_limits<T>::epsilon() * 1e3) {
    if constexpr (std::is_integral_v<T>) return a == b;
    else {
        if (a == b) return true;
        
        T diff = std::abs(a - b);
        T norm = std::numeric_limits<T>::min();

        if (std::abs(a) < norm && std::abs(b) < norm) {
            return diff < (eps * norm);
        }

        return diff <= eps * std::max(std::abs(a), std::abs(b));
    }
}

#endif
#ifndef TYPE_FUNC
#define TYPE_FUNC


#include <limits>
#include <cmath>
#include <type_traits> /* típus kategória vizsgálathoz */

template<typename T> bool type_numeric_eq(const T& a, const T& b) {
    if constexpr (std::is_integral_v<T>) return a == b;
    else {
        if (a == b) return true;
        
        T eps = std::numeric_limits<T>::epsilon();
        T diff = std::abs(a - b);
        T norm = std::min(std::abs(a) + std::abs(b), std::numeric_limits<T>::max());

        return diff < std::max(eps, eps * norm);
    }
}

#endif
#ifndef CUSTOM_TYPE_TRAITS
#define CUSTOM_TYPE_TRAITS

#include <type_traits>

#include "error_base.hpp"
#include "hypercomplex_base.hpp"

namespace genMatrix::internal::type_traits {
/** Hibaosztály vizsgálat */

template <typename T>
constexpr bool __is_error_type() {
    return std::is_base_of_v<genMatrix::internal::types::genMatrix_Error, T>;
}

/**
 * @brief Igaz, ha a típus egy genMatrix hibaosztály.
 */
template<typename T>
constexpr bool is_error_type = __is_error_type<T>();

/** Hiperkomplex vizsgálat */

template <typename T>
constexpr bool __is_hypercomplex() {
    return std::is_base_of_v<genMatrix::internal::types::Hypercomplex, T>;
}

/**
 * @brief Igaz, ha a típus egy hiperkomplex szám.
 */
template<typename T>
constexpr bool is_hypercomplex = __is_hypercomplex<T>();

template <typename T>
constexpr bool __is_hypercomplex2() {
    return std::is_base_of_v<genMatrix::internal::types::Hypercomplex2D, T>;
}

/**
 * @brief Igaz, ha a típus egy 2 dimenziós hiperkomplex szám.
 */
template<typename T>
constexpr bool is_hypercomplex2 = __is_hypercomplex2<T>();


/** Pontosság vizsgálat */

template <typename T>
constexpr bool __has_double_precision() {
    if constexpr (std::is_same_v<T, double> || is_hypercomplex2<T>) 
        return true;
}

/**
 * @brief Igaz, ha a típus változói double alapúak.
 */
template<typename T>
constexpr bool has_double_precision = __has_double_precision<T>();



}

#endif
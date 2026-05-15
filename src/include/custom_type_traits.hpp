/**
 * @file custom_type_traits.hpp
 * @author Kovács Botond
 * @brief Sablonok típusok vagy azok tulajdonságainak vizsgálatához.
 */
#ifndef CUSTOM_TYPE_TRAITS_H
#define CUSTOM_TYPE_TRAITS_H

#include <type_traits>

#include "error_base.hpp"
#include "hypercomplex_base.hpp"

namespace genMatrix::internal::type_traits {
/** ------------------------ Hibaosztály vizsgálat ------------------------- **/

template <typename T>
constexpr bool __is_error_type() {
    return std::is_base_of_v<genMatrix::internal::types::genMatrix_Error, T>;
}

/**
 * @brief Igaz, ha a típus egy genMatrix hibaosztály.
 */
template<typename T>
constexpr bool is_error_type = __is_error_type<T>();

/** ----------------------------------------------------------------------- **/
/** ----------------------- Hiperkomplex vizsgálat ------------------------ **/

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

/** ----------------------------------------------------------------------- **/
/** ------------------------ Pontosság vizsgálat -------------------------- **/

template <typename T>
constexpr bool __has_no_precision() {
    if constexpr (std::is_integral_v<T>) 
        return true;
    else
        return false;
}

/**
 * @brief Igaz, ha a típus változói csak egészet tudnak tárolni.
 */
template<typename T>
constexpr bool has_no_precision = __has_no_precision<T>();


template <typename T>
constexpr bool __has_float_precision() {
    if constexpr (std::is_same_v<T, float>) 
        return true;
    else
        return false;
}

/**
 * @brief Igaz, ha a típus változói float alapúak.
 */
template<typename T>
constexpr bool has_float_precision = __has_float_precision<T>();

template <typename T>
constexpr bool __has_double_precision() {
    if constexpr (std::is_same_v<T, double> || is_hypercomplex2<T>) 
        return true;
    else
        return false;
}

/**
 * @brief Igaz, ha a típus változói double alapúak.
 */
template<typename T>
constexpr bool has_double_precision = __has_double_precision<T>();

/**
 * @brief Igaz, ha a típus lebegőpontos.
 */
template<typename T>
constexpr bool has_precision = __has_double_precision<T>() || __has_float_precision<T>();

/** ----------------------------------------------------------------------  **/
/** ----------------- Típusok közti műveletek vizsgálata -----------------  **/

/** Típusok összeadását ellenőrző sablon */
template <typename T, typename S, typename = void>
struct has_add : std::false_type {};

template <typename T, typename S>
struct has_add<T, S, std::void_t<decltype(std::declval<T>() + std::declval<S>())>> 
    : std::true_type {};

/** 
 * @brief Igaz, ha a két típust össze lehet adni. 
 */
template <typename T, typename S>
constexpr bool has_add_v = has_add<T, S>::value;


/** Típusok szorzását ellenőrző sablon */
template <typename T, typename S, typename = void>
struct has_mul : std::false_type {};

template <typename T, typename S>
struct has_mul<T, S, std::void_t<decltype(std::declval<T>() * std::declval<S>())>> 
    : std::true_type {};

/**
 * @brief Igaz, ha a két típust össze lehet szorzoni. 
 */
template <typename T, typename S>
constexpr bool has_mul_v = has_mul<T, S>::value;

/** ----------------------------------------------------------------------  **/
}

#endif
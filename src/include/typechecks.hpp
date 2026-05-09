/**
 * @file typechecks.hpp
 * @author Kovács Botond
 * @brief Típusokon végzett ellenőrzések sablonjai, az operátor SFINAE alapja.
 */
#ifndef TYPECHECKS_H
#define TYPECHECKS_H

#include <type_traits>
#include <utility>

/** Típusok összeadását ellenőrző sablon */
template <typename T, typename S, typename = void>
struct has_add : std::false_type {};

template <typename T, typename S>
struct has_add<T, S, std::void_t<decltype(std::declval<T>() + std::declval<S>())>> 
    : std::true_type {};

/** Igaz, ha a két típust össze lehet adni. */
template <typename T, typename S>
constexpr bool has_add_v = has_add<T, S>::value;


/** Típusok szorzását ellenőrző sablon */
template <typename T, typename S, typename = void>
struct has_mul : std::false_type {};

template <typename T, typename S>
struct has_mul<T, S, std::void_t<decltype(std::declval<T>() * std::declval<S>())>> 
    : std::true_type {};

/** Igaz, ha a két típust össze lehet szorzoni.. */
template <typename T, typename S>
constexpr bool has_mul_v = has_mul<T, S>::value;



#endif

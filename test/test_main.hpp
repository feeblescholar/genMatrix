/**
 * @file test_main.hpp
 * @author Kovács Botond
 * @brief A teszteléshez használt fő fejlécfájl.
 */
#ifndef TEST_MAIN_H
#define TEST_MAIN_H

#include <gtest/gtest.h>
#include "../lib/memtrace_gtest/memtrace.h"

#include "test_utils.hpp"

#include "../src/include/genMatrix.hpp"

/**
 * @brief A gépi epszilon, azaz a típus értékei közötti legkisebb reprezentálható
 *        tavolság.
 */
template<typename T>
double EPS = std::numeric_limits<T>::epsilon();

/**
 * @brief A gépi epszilon ezerszerese.
 */
template<typename T>
double EPS_L = EPS<T> * 1e3;

#endif
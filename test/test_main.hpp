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

template<typename T>
double EPS = std::numeric_limits<T>::epsilon();

template<typename T>
double EPS_L = std::numeric_limits<T>::epsilon() * 1e3;

#endif
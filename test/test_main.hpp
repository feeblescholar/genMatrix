/**
 * @file test_main.hpp
 * @brief A tesztfájlokat összefogó header fájl, továbbá itt lehet a típusteszteket beállítani.
 */

#ifndef TEST_MAIN_H
#define TEST_MAIN_H

#include <gtest/gtest.h>

#include "../src/include/genMatrix.hpp"
#include "../lib/memtrace_gtest/memtrace.h"
#include "test_utils.hpp"

/**
 * Dummy osztály a gtest-hez, a templatekhez példányosításához szükséges
 */
template<typename T> class MatrixTest : public ::testing::Test {};

typedef ::testing::Types<int, double, float, long> TestTypes;
TYPED_TEST_SUITE(MatrixTest, TestTypes);

#endif
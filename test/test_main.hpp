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
 * @brief A random tesztértékek alsó határa. 
 */
const int lbound = -12;

/** 
 * @brief A random tesztértékek felső határa. 
 */
const int ubound = 20;  

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

/**
 * Itt *NEM* kell a teszttípusokat megadni, ez a memtrace makróháborújához kell.
 */
typedef ::testing::Types<void> NoType;

/**
 * @class MatrixBase
 * @brief Dummy osztály a gtest-hez, a memtracenek kell.
 */
template<typename T> class MatrixBase : public ::testing::Test {};
TYPED_TEST_SUITE(MatrixBase, NoType);

/**
 * @class VectorBase
 * @brief Dummy osztály a gtest-hez, a memtracenek kell.
 */
template<typename T> class VectorBase : public ::testing::Test {};
TYPED_TEST_SUITE(VectorBase, NoType);

#endif
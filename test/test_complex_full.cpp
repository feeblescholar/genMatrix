#include <gtest/gtest.h>
#include "test_main.hpp"

/**
 * Le kell tiltani itt memtrace-t mert szereti a GoogleTestet 
 * Nincs itt (saját) dinamikus erőforrás.
 */

#ifdef new
#undef new
#endif
#ifdef delete
#undef delete
#endif

GTEST_TEST(ComplexTest, GetterSetter) {
    genMatrix::Complex z(1, 2);
    genMatrix::Complex z1(1);
    genMatrix::Complex z2;

    EXPECT_EQ(1, z.getRe());
    EXPECT_EQ(1, z1.getRe());
    EXPECT_EQ(0, z2.getRe());
    EXPECT_EQ(2, z.getIm());
    EXPECT_EQ(0, z1.getIm());
    EXPECT_EQ(0, z2.getIm());

    new char[1];
}

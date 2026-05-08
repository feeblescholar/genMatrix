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

TEST(ComplexTest, GetterSetter) {
    genMatrix::Complex z(1, 2);
    genMatrix::Complex z1(1);
    genMatrix::Complex z2;

    EXPECT_EQ(1, z.getRe());
    EXPECT_EQ(1, z1.getRe());
    EXPECT_EQ(0, z2.getRe());
    EXPECT_EQ(2, z.getIm());
    EXPECT_EQ(0, z1.getIm());
    EXPECT_EQ(0, z2.getIm());

    z1.setRe(7);
    z1.setIm(-1);

    EXPECT_EQ(7, z1.getRe());
    EXPECT_EQ(-1, z1.getIm());
}

TEST(ComplexTest, AbsoluteValue) {
    genMatrix::Complex z = rng<genMatrix::Complex>;

    double exp = std::hypot(z.getRe(), z.getIm());
    double act = z.abs();

    EXPECT_NEAR(exp, act, eps_l<double>);
}

TEST(ComplexTest, Comparisons) {
    genMatrix::Complex z(1, 2);
    genMatrix::Complex z1(2, 1);

    EXPECT_TRUE(z == z);
    EXPECT_FALSE(z != z);
    EXPECT_TRUE(z != z1);
    EXPECT_FALSE(z == z1);
}

TEST(ComplexTest, Conjugate) {
    genMatrix::Complex z = random<genMatrix::Complex>();
    
    genMatrix::Complex exp(z.getRe(), -1 * z.getIm());
    genMatrix::Complex act = ~z;

    EXPECT_NEAR(exp.getRe(), act.getRe(), eps_l<double>);
    EXPECT_NEAR(exp.getIm(), act.getIm(), eps_l<double>);
}
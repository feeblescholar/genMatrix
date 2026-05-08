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
    genMatrix::Complex z = rng<genMatrix::Complex>;
    
    genMatrix::Complex exp(z.getRe(), -1 * z.getIm());
    genMatrix::Complex act = ~z;

    EXPECT_NEAR(exp.getRe(), act.getRe(), eps_l<double>);
    EXPECT_NEAR(exp.getIm(), act.getIm(), eps_l<double>);
}

TEST(ComplexTest, Operators) {
    genMatrix::Complex a = rng<genMatrix::Complex>;
    genMatrix::Complex b = rng<genMatrix::Complex>;

    genMatrix::Complex aC = a - b;
    aC += b;
    aC = a + b;
    aC -= b;

    genMatrix::Complex bC = b * a;
    bC /= a;
    bC = b / a;
    bC *= a;

    EXPECT_TRUE(a == aC);
    EXPECT_TRUE(b == bC);

    double r1 = rng<double>;
    double r2 = rng<double>;

    aC = a - r1;
    aC += r1;
    aC = a + r2;
    aC -= r2;

    bC = b * r1;
    bC /= r1;
    bC = b / r2;
    bC *= r2;

    EXPECT_TRUE(a == aC);
    EXPECT_TRUE(b == bC);
}

TEST(ComplexTest, DivByZero) {
    genMatrix::Complex z = rng<genMatrix::Complex>;

    EXPECT_THROW(z / 0, std::domain_error);
    EXPECT_THROW(z /= 0, std::domain_error);
}
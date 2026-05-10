#include <gtest/gtest.h>
#include "../src/include/genMatrix.hpp"
#include "../lib/memtrace_gtest/memtrace.h"

#undef new
#undef delete


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

TEST(sa, s) {
    using genMatrix::Complex;
    genMatrix::Matrix<Complex> M(3, 3);
    M << Complex(0, 1), Complex(1, 0), Complex(), Complex(2, 0), Complex(0, -1), Complex(1, 0),
    Complex(), Complex(3, 0), Complex(0, 1);

    Complex dM = Complex(0, -4);
    std::cout << dM << " " << det(M) << std::endl;
    EXPECT_NEAR(dM.getRe(), det(M).getRe(), std::numeric_limits<double>::epsilon() * 1e3);
    EXPECT_NEAR(dM.getIm(), det(M).getIm(), std::numeric_limits<double>::epsilon() * 1e3);
}
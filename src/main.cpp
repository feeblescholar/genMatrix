#include "include/genMatrix.hpp"
#include "../lib/memtrace_gtest/gtest_lite.h"

#include <iostream>

using namespace genMatrix;

int main(void) {
    /**
     * Oldjuk meg a következő egyenletrendszert!
     * 2x +  y -  z + 3w +  v = 7
     * -x + 4y + 2z -  w + 5v = 3
     * 3x - 2y +  z + 2w -  v = 10
     * x +  y +  z +  w +  v = 8
     * 5x - 3y + 4z - 2w + 2v = 1
     */
    TEST(genMatrixMain, LinearSystemOfEquations)
        Matrix<int> A(5, 5);
        A <<  2,  1, -1,  3,  1,
             -1,  4,  2, -1,  5,
              3, -2,  1,  2, -1,
              1,  1,  1,  1,  1,
              5, -3,  4, -2,  2;

        Vector<int> B(5);
        B << 7, 3, 10, 8, 1;

        /** Ainv * B a megoldás */
        auto Ainv = A.inverse();
        auto sol = Ainv * (Matrix<int>)B;

        /** Az elvárt megoldás */
        double x = 0.08045977011494253;
        double y = 1.793103448275862;
        double z = 4.218390804597701;
        double w = 3.67816091954023;
        double v = -1.7701149425287357;

        EXPECT_DOUBLE_EQ(x, sol(0, 0));
        EXPECT_DOUBLE_EQ(y, sol(1, 0));
        EXPECT_DOUBLE_EQ(z, sol(2, 0));
        EXPECT_DOUBLE_EQ(w, sol(3, 0));
        EXPECT_DOUBLE_EQ(v, sol(4, 0));
    END
    
    return 0;
}
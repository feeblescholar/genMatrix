#include "genMatrix.hpp"
#include "memtrace_gtest/memtrace.h"
#include "memtrace_gtest/gtest_lite.h"

int main(int argc, char **argv) {

    using namespace genMatrix;

    TEST("Meretek", "genMatrix<int> statikus")
        Matrix<int> M0(5, 6);
        EXPECT_EQ((size_t)5, M0.getRows());
        EXPECT_EQ((size_t)6, M0.getCols());
        EXPECT_EQ((size_t)30, M0.size());

        Matrix<int> M1(0, 7);
        EXPECT_EQ((size_t)0, M1.getRows());
        EXPECT_EQ((size_t)7, M1.getCols());
        EXPECT_EQ((size_t)7, M1.size());

        Matrix<int> M2(3, 0);
        EXPECT_EQ((size_t)3, M2.getRows());
        EXPECT_EQ((size_t)0, M2.getCols());
        EXPECT_EQ((size_t)3, M2.size());

        Matrix<int> M3;
        EXPECT_EQ((size_t)0, M3.size());
    END

    TEST("Eigen helyes", "genMatrix<int> statikus")
        Matrix<int> M(2, 2);
        EXPECT_NO_THROW((M << 1, -2, -3, 4));
    END

    TEST("Eigen keves parameter", "genMatrix<int> statikus")
        Matrix<int> M(3, 3);
        EXPECT_THROW((M << 1, 2, 3, -4), const char*);
    END

    TEST("Eigen sok parameter", "genMatrix<int> statikus")
        Matrix<int> M(3, 3);
        EXPECT_THROW((M << 1, 2, 3, 4, -5, 6, 7, 8, 9, 10), const char*);
    END

    TEST("Eigen dinamikusra", "genMatrix<int> statikus")
        Matrix<int> M;
        EXPECT_THROW((M << 1, 2, 3, 4), const char*);
    END

    Matrix<int> M(4, 4);
    M << 1, 2, 3, 4, 
         5, -6, 7, 8, 
         9, 10, 11, -12,
         13, 14, 15, 16;

    TEST("Indexeles", "genMatrix<int> statikus")
        EXPECT_NO_THROW(EXPECT_EQ((int)1, M(0, 0)));
        EXPECT_NO_THROW(EXPECT_EQ((int)16, M(3, 3)));
        EXPECT_NO_THROW(EXPECT_EQ((int)-12, M(2, 3)));
        EXPECT_NO_THROW(EXPECT_EQ((int)10, M(2, 1)));
    END

    TEST("Tulindexeles", "genMatrix<int> statikus")
        EXPECT_THROW(M(-1, 0), const char*);
        EXPECT_THROW(M(76, 0), const char*);
        EXPECT_THROW(M(0, -1), const char*);
        EXPECT_THROW(M(0, 76), const char*);
        EXPECT_THROW(M(-1, -1), const char*);
        EXPECT_THROW(M(76, 84), const char*);
    END

    TEST("Osszehasonlitasok", "genMatrix<int> statikus")
        Matrix<int> M_cp = M;
        Matrix<int> L(2, 2);
        L << 1, 2, 
             3, -7;
        Matrix<int> A(3, 2);
        A << -4, -6,
             -6, 56,
             72, 32;

        Matrix<int> D;
        Matrix<int> D2;

        EXPECT_TRUE(A == A);
        EXPECT_TRUE(M == M_cp);
        EXPECT_TRUE(D == D2);

        EXPECT_FALSE(M_cp == L);
        EXPECT_FALSE(D == L);
        EXPECT_FALSE(L == A);
    END

    TEST("Masolas kitoltott matrixon", "genMatrix<int> statikus")
        Matrix<int> M_cp = M;
        EXPECT_TRUE(M_cp == M);
    END

    TEST("Ertekadas onmagan", "genMatrix<int> statikus")
        Matrix<int> M_cp = M;
        M_cp = M_cp;
        EXPECT_TRUE(M_cp == M);
    END

    TEST("Tobbszoros ertekadas", "genMatrix<int> statikus")
        Matrix<int> M1 = M;
        Matrix<int> L(2, 2);
        L << 1, 2, 
             3, -7;
        Matrix<int> U(0, 2);
        U << 1, 
             3;
        Matrix<int> A(3, 2);
        A << -4, -6,
             -6, 56,
             72, 32;

        M1 = L;
        EXPECT_TRUE(M1 == L);

        M1 = U;
        EXPECT_TRUE(M1 == U);

        M1 = A;
        EXPECT_TRUE(M1 == A);
    END

    TEST("Sor- es oszlopcsere", "genMatrix<int> statikus")
        Matrix<int> M1(4, 3);
        M1 << -1, 2, 3,
              4, -5, 6,
              7, 8, 9,
              10, 11, 12;
        Matrix<int> M2(4, 3);
        M2 << 4, -5, 6,
              -1, 2, 3,
              7, 8, 9,
              10, 11, 12;
        Matrix<int> M3 = M1;
        Matrix<int> M4(4, 3);
        M4 << 6, -5, 4,
              3, 2, -1,
              9, 8, 7,
              12, 11, 10;
        Matrix<int> M5 = M2;
        Matrix<int> M6(4, 3);
        M6 << 9, 8, 7,
              6, -5, 4,
              3, 2, -1,
              12, 11, 10;

        M1.swapRow(0, 1);
        EXPECT_TRUE(M1 == M2);

        M1.swapRow(0, 1);
        EXPECT_TRUE(M1 == M3) << "valtozott valami sorcsere kozben";

        EXPECT_THROW(M1.swapRow(-1, 62), const char*);

        M2.swapCol(0, 2);
        EXPECT_TRUE(M2 == M4);
        
        M2.swapCol(0, 2);
        EXPECT_TRUE(M2 == M5) << "valtozott valami sorcsere kozben";

        EXPECT_THROW(M2.swapCol(19, -62), const char*);

        M1.swapRow(0, 2).swapCol(0, 2);
        EXPECT_TRUE(M1 == M6);
    END

    return 0;
}
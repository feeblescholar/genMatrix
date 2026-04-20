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

    TEST("Szorzas konstanssal", "genMatrix<int> statikus")
        Matrix<int> M1 = M, M2 = M;
        M1 *= -7;
        M2 *= 4;

        for (size_t i = 0; i < M1.getCols(); i++) {
            for (size_t k = 0; k < M1.getRows(); k++) {
                EXPECT_EQ(M(i, k) * -7, M1(i, k));
                EXPECT_EQ(M(i, k) * 4, M2(i, k));
            }
        }
    END 

    TEST("Osszeadas, kivonas", "genMatrix<int> statikus")
        Matrix<int> M1 = M, M2 = M1 + 1, M3 = M1 + 9, M4 = M1, M5 = M1, M6 = M2 + M3, M7 = M - M1;
        M4 += 9;
        M5 -= 1;

        for (size_t i = 0; i < M1.getCols(); i++) {
            for (size_t k = 0; k < M1.getRows(); k++) {
                EXPECT_EQ(M1(i, k) + 1, M2(i, k));
                EXPECT_EQ(M1(i, k) + 9, M3(i, k));
                EXPECT_EQ(M2(i, k) + M3(i, k), M6(i, k));
                EXPECT_EQ((int)0, M7(i, k));
            }
        }

        EXPECT_TRUE(M4 == M3);
        EXPECT_TRUE(M5 == M2);    
    END

    TEST("Matrixszorzas", "genMatrix<int> statikus")
        Matrix<int> M1(3, 4);
        M1 << 1, 0, 2, 1,
              0, 3, 1, 2,
              2, 1, 0, 1;

        Matrix<int> M2(4, 3);
        M2 << 1, 2, 0,
              0, 1, 1,
              2, 0, 3,
              1, 1, 2;

        Matrix<int> M12(3, 3);
        M12 << 6, 3, 8,
               4, 5, 10,
               3, 6, 3;

        Matrix<int> M21(4, 4);
        M21 << 1, 6, 4, 5,
               2, 4, 1, 3,
               8, 3, 4, 5,
               5, 5, 3, 5;

        EXPECT_TRUE((M1 * M2) == M12);
        EXPECT_TRUE((M2 * M1) == M21);
    END

    TEST("Transzponal negyzetes matrix", "genMatrix<int> statikus")
        Matrix<int> N(2, 2);
        N << 1, 2, 
             3, 4;

        Matrix<int> L = N.transpose();

        Matrix<int> U = N;
        U.transposeInPlace();

        Matrix<int> TN(2, 2);
        TN << 1, 3, 
              2, 4;

        EXPECT_TRUE(TN == L);
        EXPECT_TRUE(TN == U);
    END

    TEST("Transzponal altalanos matrix", "genMatrix<int> statikus")
        Matrix<int> N(2, 3);
        N << 1, 2, 3,
             4, 5, 6;

        Matrix<int> L = N.transpose();

        Matrix<int> U = N;
        U.transposeInPlace();

        Matrix<int> TN(3, 2);
        TN << 1, 4, 
              2, 5,
              3, 6;

        EXPECT_TRUE(TN == L);
        EXPECT_TRUE(TN == U);
    END

    return 0;
}
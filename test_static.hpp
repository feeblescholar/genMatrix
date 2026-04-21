#ifndef TESTGEN
#define TESTGEN

#include <random>
#include <type_traits>

#include "genMatrix.hpp"
#include "memtrace_gtest/gtest_lite.h"
#include "memtrace_gtest/memtrace.h"

namespace genMatrix::genMatrixTest {
    template<typename T, bool neg = true> T random() {
        int lbound = -1000;
        if (!neg) lbound = 0;
        const int ubound = 1000;

        std::default_random_engine eng(std::random_device{}());

        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> d(lbound, ubound);
            return d(eng);
        }
        else {
            std::uniform_real_distribution<T> d(lbound, ubound);
            return d(eng);
        }
    }

    /**
     * Egy fix tömb, random számokkal.
     * Kizárólag teszteléshez van, sok mindent nem ellenőriz.
     */
    template<typename T, size_t n = 1, bool neg = true> struct TestArray {
        T arr[n];
        size_t s;

        TestArray() : s(n) {
            for (size_t i = 0; i < n; i++) arr[i] = random<T, neg>();
        }

        T& operator[](const size_t idx) {
            if (idx >= s) throw "tulindexeles";
            return arr[idx]; 
        }

        const T& operator[](const size_t idx) const {
            if (idx >= s) throw "tulindexeles";
            return arr[idx]; 
        }

        void fillmat(genMatrix::Matrix<T>& mtx){
            if (s > mtx.size()) throw "nem fer bele";
            for (size_t k = 0; k < mtx.size(); k++) {
                size_t i = k / mtx.getCols();
                size_t j = k % mtx.getCols();
                mtx(i, j) = arr[k];
            }
        }
    };

    template<typename T> void size_calculation__static() {
        using namespace genMatrix;

        TEST("Rows, colums, size", "Size Calculation")
            TestArray<size_t, 4, false> t;
        
            Matrix<int> M0(t[0], t[1]);
            EXPECT_EQ((size_t)t[0], M0.getRows());
            EXPECT_EQ((size_t)t[1], M0.getCols());
            EXPECT_EQ((size_t)t[0] * t[1], M0.size());

            Matrix<T> M1(0, t[2]);
            EXPECT_EQ((size_t)0, M1.getRows());
            EXPECT_EQ((size_t)t[2], M1.getCols());
            EXPECT_EQ((size_t)t[2], M1.size());

            Matrix<T> M2(t[3], 0);
            EXPECT_EQ((size_t)t[3], M2.getRows());
            EXPECT_EQ((size_t)0, M2.getCols());
            EXPECT_EQ((size_t)t[3], M2.size());
        END
    }

    template<typename T> void eigen_fill__static() {
        using namespace genMatrix;

        TEST("Eigen", "Eigen fill")
            TestArray<T, 4> t;
            Matrix<T> M(2, 2);
            EXPECT_NO_THROW((M << t[0], t[1], t[2], t[3]));
        END

        TEST("Eigen <size", "Eigen fill")
            TestArray<T, 4> t;
            Matrix<T> M(3, 3);
            EXPECT_THROW((M << t[0], t[1], t[2], t[3]), const char*);
        END

        TEST("Eigen >size", "Eigen fill")
            TestArray<T, 4> t;
            Matrix<T> M(2, 2);
            EXPECT_THROW((M << t[0], t[1], t[2], t[3], t[0]), const char*);
        END
    }

    template<typename T> void indexing__static() {
        using namespace genMatrix;

        TestArray<T, 12> t;
        Matrix<T> M(3, 4);
        t.fillmat(M);

        TEST("Indexing", "Indexing")
            EXPECT_NO_THROW(EXPECT_EQ((T)t[0], M(0, 0)));
            EXPECT_NO_THROW(EXPECT_EQ((T)t[11], M(2, 3)));
            EXPECT_NO_THROW(EXPECT_EQ((T)t[7], M(1, 3)));
            EXPECT_NO_THROW(EXPECT_EQ((T)t[9], M(2, 1)));
        END

        TEST("Invalid indexing", "Indexing")
            EXPECT_THROW(M(-1, 0), const char*);
            EXPECT_THROW(M(0, 4), const char*);
            EXPECT_THROW(M(0, -1), const char*);
            EXPECT_THROW(M(0, 76), const char*);
            EXPECT_THROW(M(-1, -1), const char*);
            EXPECT_THROW(M(76, 84), const char*);
        END
    }

    template<typename T> void comparisons__static() {
        using namespace genMatrix;

        TEST("Comparisons", "Comparisons")
            TestArray<T, 40> t;
            TestArray<T, 4> t1;
            TestArray<T, 6> t2;

            Matrix<T> M(5, 8);
            t.fillmat(M);

            Matrix<T> M_cp(5, 8);
            t.fillmat(M_cp);

            Matrix<T> L(2, 2);
            t1.fillmat(L);

            Matrix<T> A(3, 2);
            t2.fillmat(A);

            Matrix<T> D;
            Matrix<T> D2;

            EXPECT_TRUE(A == A);
            EXPECT_TRUE(M == M_cp);
            EXPECT_TRUE(D == D2);

            EXPECT_FALSE(M_cp == L);
            EXPECT_FALSE(D == L);
            EXPECT_FALSE(L == A);
        END
    }

    template<typename T> void copy_assign__static() {
        using namespace genMatrix;

        TestArray<T, 24> m;
        Matrix<T> M(4, 6);
        m.fillmat(M);

        TEST("Copy filled matrix", "Copy-assign")
            Matrix<T> M_cp = M;
            EXPECT_TRUE(M_cp == M);
        END

        TEST("Self assign", "Copy-assign")
            Matrix<T> M_cp = M;
            M_cp = M_cp;
            EXPECT_TRUE(M_cp == M);
        END

        TEST("Reassign, Multi-assign", "Copy-assign")
            Matrix<T> M1 = M;

            TestArray<T, 4> l;
            Matrix<T> L(2, 2);
            l.fillmat(L);
            
            TestArray<T, 2> u;
            Matrix<T> U(1, 2);
            u.fillmat(U);

            TestArray<T, 6> a;
            Matrix<T> A(3, 2);
            a.fillmat(A);

            M1 = L;
            EXPECT_TRUE(M1 == L);

            M1 = U;
            EXPECT_TRUE(M1 == U);

            M1 = A = L;
            EXPECT_TRUE(M1 == A);
            EXPECT_TRUE(M1 == L);
            EXPECT_TRUE(A == L);
        END
    }

    template<typename T> void swapping__static() {
        using namespace genMatrix;

        TEST("Swap rows and columns", "Swapping")
            TestArray<T, 12> m1;
            Matrix<T> M1(4, 3);
            m1.fillmat(M1);
            Matrix<T> M3 = M1;
            
            Matrix<T> M2(4, 3);
            M2 << M1(1, 0), M1(1, 1), M1(1, 2), 
                  M1(0, 0), M1(0, 1), M1(0, 2),
                  M1(2, 0), M1(2, 1), M1(2, 2),
                  M1(3, 0), M1(3, 1), M3(3, 2);
            Matrix<T> M5 = M2;

            Matrix<T> M4(4, 3);
            M4 << M2(0, 2), M2(0, 1), M2(0, 0), 
                  M2(1, 2), M2(1, 1), M2(1, 0),
                  M2(2, 2), M2(2, 1), M2(2, 0),
                  M2(3, 2), M2(3, 1), M2(3, 0);
            
            Matrix<T> M6(4, 3);
            M6 << M1(2, 2), M1(2, 1), M1(2, 0), 
                  M1(1, 2), M1(1, 1), M1(1, 0),
                  M1(0, 2), M1(0, 1), M1(0, 0),
                  M1(3, 2), M1(3, 1), M3(3, 0);

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
    }

    template<typename T> void binary_operands__static() {

        TEST("Constant multiplier", "Binary operands")
            TestArray<T, 32> m;
            Matrix<T> M(4, 8);
            m.fillmat(M);

            Matrix<T> M1 = M, M2 = M;
            M1 *= -7;
            M2 *= 4;

            for (size_t i = 0; i < M1.getRows(); i++) {
                for (size_t k = 0; k < M1.getCols(); k++) {
                    EXPECT_EQ(M(i, k) * -7, M1(i, k));
                    EXPECT_EQ(M(i, k) * 4, M2(i, k));
                }
            }
        END 

        TEST("Addition, subtraction", "Binary operands")
            TestArray<T, 16> m;
            Matrix<T> M(4, 4);
            m.fillmat(M);

            Matrix<T> M1 = M, M2 = M1 + 1, M3 = M1 + 9, M4 = M1, M5 = M1, M6 = M2 + M3, M7 = M - M1;
            M4 += 9;
            M5 -= 1;

            for (size_t i = 0; i < M1.getCols(); i++) {
            for (size_t k = 0; k < M1.getRows(); k++) {
                    EXPECT_EQ(M1(i, k) + 1, M2(i, k));
                    EXPECT_EQ(M1(i, k) + 9, M3(i, k));
                    EXPECT_EQ(M2(i, k) + M3(i, k), M6(i, k));
                    EXPECT_EQ((T)0, M7(i, k));
                }
            }

            EXPECT_TRUE(M4 == M3);
            EXPECT_TRUE(M5 == M2);    
        END

        TEST("Multiply matrices", "Binary operands")
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
    }

    template<typename T> void transpose_static() {
        using namespace genMatrix;

        TEST("Transpose square matrix", "Transpose")
            TestArray<T, 4> n;
            Matrix<T> N(2, 2);
            n.fillmat(N);
            
            Matrix<T> L = N.transpose();

            Matrix<T> U = N;
            U.transposeInPlace();

            Matrix<T> TN(2, 2);
            TN << n[0], n[2], 
                  n[1], n[3];

            EXPECT_TRUE(TN == L);
            EXPECT_TRUE(TN == U);
        END

        TEST("Transpose general", "Transpose")
            TestArray<T, 6> n;
            Matrix<T> N(2, 3);
            n.fillmat(N);

            Matrix<T> L = N.transpose();

            Matrix<T> U = N;
            U.transposeInPlace();

            Matrix<T> TN(3, 2);
            TN << n[0], n[3],
                  n[1], n[4],
                  n[2], n[5];
            
            EXPECT_TRUE(TN == L);
            EXPECT_TRUE(TN == U);
        END
    }

    template<typename T> void full_genMatrix_test__static() {
        std::cout << "\n==== genMatrix test on: " << typeid(T).name() << " ====\n";

        size_calculation__static<T>();
        eigen_fill__static<T>();
        indexing__static<T>();
        comparisons__static<T>();
        copy_assign__static<T>();
        swapping__static<T>();
        binary_operands__static<T>();
        transpose_static<T>();

        std::cout << "\n===============================\n";
    }
}

#endif
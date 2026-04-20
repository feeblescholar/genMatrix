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
        EXPECT_NO_THROW((M << 1, 2, 3, 4));
    END

    TEST("Eigen keves parameter", "genMatrix<int> statikus")
        Matrix<int> M(3, 3);
        EXPECT_THROW((M << 1, 2, 3, 4), const char*);
    END

    TEST("Eigen sok parameter", "genMatrix<int> statikus")
        Matrix<int> M(3, 3);
        EXPECT_THROW((M << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10), const char*);
    END

    TEST("Eigen dinamikusra", "genMatrix<int> statikus")
        Matrix<int> M;
        EXPECT_THROW((M << 1, 2, 3, 4), const char*);
    END

    Matrix<int> M(4, 4);
    M << 1, 2, 3, 4, 
         5, 6, 7, 8, 
         9, 10, 11, 12,
         13, 14, 15, 16;

    TEST("Indexeles", "genMatrix<int> statikus")
        EXPECT_NO_THROW(EXPECT_EQ((int)1, M(0, 0)));
        EXPECT_NO_THROW(EXPECT_EQ((int)16, M(3, 3)));
        EXPECT_NO_THROW(EXPECT_EQ((int)12, M(2, 3)));
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


    return 0;
}
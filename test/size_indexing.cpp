#include "test_main.hpp"

using namespace genMatrix;

TYPED_TEST(MatrixTest, SizeCalculation) {
    TestArray<size_t, 4, false> t;

    Matrix<TypeParam> M0(t[0], t[1]);
    EXPECT_EQ((size_t)t[0], M0.getRows());
    EXPECT_EQ((size_t)t[1], M0.getCols());
    EXPECT_EQ((size_t)t[0] * t[1], M0.size());

    Matrix<TypeParam> M1(1, t[2]);
    EXPECT_EQ((size_t)1, M1.getRows());
    EXPECT_EQ((size_t)t[2], M1.getCols());
    EXPECT_EQ((size_t)t[2], M1.size());

    Matrix<TypeParam> M2(t[3], 1);
    EXPECT_EQ((size_t)t[3], M2.getRows());
    EXPECT_EQ((size_t)1, M2.getCols());
    EXPECT_EQ((size_t)t[3], M2.size());
}

TYPED_TEST(MatrixTest, IndexingCorrect) {
    TestArray<TypeParam, 12> t;
    genMatrix::Matrix<TypeParam> M(3, 4);
    t.mtx_fill(M);

    EXPECT_NO_THROW(EXPECT_EQ((TypeParam)t[0], M(0, 0)));
    EXPECT_NO_THROW(EXPECT_EQ((TypeParam)t[11], M(2, 3)));
    EXPECT_NO_THROW(EXPECT_EQ((TypeParam)t[7], M(1, 3)));
    EXPECT_NO_THROW(EXPECT_EQ((TypeParam)t[9], M(2, 1)));
}

TYPED_TEST(MatrixTest, IndexingIncorrect) {
    TestArray<TypeParam, 12> t;
    genMatrix::Matrix<TypeParam> M(3, 4);
    t.mtx_fill(M);

    EXPECT_THROW(M(-1, 0), genMatrix::Matrix_Error);
    EXPECT_THROW(M(0, 4), genMatrix::Matrix_Error);
    EXPECT_THROW(M(0, -1), genMatrix::Matrix_Error);
    EXPECT_THROW(M(0, 76), genMatrix::Matrix_Error);
    EXPECT_THROW(M(-1, -1), genMatrix::Matrix_Error);
    EXPECT_THROW(M(76, 84), genMatrix::Matrix_Error);
}
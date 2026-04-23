#include "test_main.hpp"

TYPED_TEST(MatrixTest, ConstantTimesMatrix) {
    using namespace genMatrix;

    TestArray<TypeParam, 32> m;
    Matrix<TypeParam> M(4, 8);
    m.fillmat(M);

    Matrix<TypeParam> M1 = M, M2 = M;
    M1 *= -7;
    M2 *= 4;

    for (size_t i = 0; i < M1.getRows(); i++) {
        for (size_t k = 0; k < M1.getCols(); k++) {
            EXPECT_EQ(M(i, k) * -7, M1(i, k));
            EXPECT_EQ(M(i, k) * 4, M2(i, k));
        }
    }
}

TYPED_TEST(MatrixTest, AdditionSubtraction) {
    using namespace genMatrix;

    TestArray<TypeParam, 16> m;
    Matrix<TypeParam> M(4, 4);
    m.fillmat(M);

    Matrix<TypeParam> M1 = M, M2 = M1 + 1, M3 = M1 + 9, M4 = M1, M5 = M1, M6 = M2 + M3, M7 = M - M1;
    M4 += 9;
    M5 -= 1;

    for (size_t i = 0; i < M1.getCols(); i++) {
        for (size_t k = 0; k < M1.getRows(); k++) {
            EXPECT_EQ(M1(i, k) + 1, M2(i, k));
            EXPECT_EQ(M1(i, k) + 9, M3(i, k));
            EXPECT_EQ(M2(i, k) + M3(i, k), M6(i, k));
            EXPECT_EQ((TypeParam)0, M7(i, k));
        }
    }

    EXPECT_TRUE(M4 == M3);
    EXPECT_TRUE(M5 == M2);    
}

TYPED_TEST(MatrixTest, MatrixMultiplication) {
    using namespace genMatrix;

    TestArray<TypeParam, 6> m1;
    Matrix<TypeParam> M1(2, 3);
    m1.fillmat(M1);

    TestArray<TypeParam, 12> m2;
    Matrix<TypeParam> M2(3, 4);
    m2.fillmat(M2);

    Matrix<TypeParam> M12(2, 4);
    /** egyszer és utoljára végigmásszuk kézzel */
    M12 << M1(0, 0) * M2(0, 0) + M1(0, 1) * M2(1, 0) + M1(0, 2) * M2(2, 0),
            M1(0, 0) * M2(0, 1) + M1(0, 1) * M2(1, 1) + M1(0, 2) * M2(2, 1),
            M1(0, 0) * M2(0, 2) + M1(0, 1) * M2(1, 2) + M1(0, 2) * M2(2, 2),
            M1(0, 0) * M2(0, 3) + M1(0, 1) * M2(1, 3) + M1(0, 2) * M2(2, 3),
            M1(1, 0) * M2(0, 0) + M1(1, 1) * M2(1, 0) + M1(1, 2) * M2(2, 0),
            M1(1, 0) * M2(0, 1) + M1(1, 1) * M2(1, 1) + M1(1, 2) * M2(2, 1),
            M1(1, 0) * M2(0, 2) + M1(1, 1) * M2(1, 2) + M1(1, 2) * M2(2, 2),
            M1(1, 0) * M2(0, 3) + M1(1, 1) * M2(1, 3) + M1(1, 2) * M2(2, 3);

    EXPECT_TRUE((M1 * M2) == M12);
}

TYPED_TEST(MatrixTest, InvalidBinaryOperations) {
    using namespace genMatrix;

    TestArray<TypeParam, 6> m1;
    TestArray<TypeParam, 24> m2;

    Matrix<TypeParam> M1(2, 3);
    m1.fillmat(M1);

    Matrix<TypeParam> M2(6, 4);
    m2.fillmat(M2);

    EXPECT_THROW((M1 + M2), genMatrix::Matrix_Error);
    EXPECT_THROW((M2 - M1), genMatrix::Matrix_Error);
    EXPECT_THROW((M1 * M2), genMatrix::Matrix_Error);
}
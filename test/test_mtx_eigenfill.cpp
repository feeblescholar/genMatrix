#include "test_main.hpp"

using namespace genMatrix;

TYPED_TEST(MatrixTest, EigenFillCorrect) {
    TestArray<TypeParam, 4> t;
    Matrix<TypeParam> M(2, 2);
    EXPECT_NO_THROW((M << t[0], t[1], t[2], t[3]));
}

TYPED_TEST(MatrixTest, EigenFillTooFew) {
    TestArray<TypeParam, 4> t;
    Matrix<TypeParam> M(3, 3);
    EXPECT_THROW((M << t[0], t[1], t[2], t[3]), genMatrix::Matrix_Error);
}

TYPED_TEST(MatrixTest, EigenFillTooMany) {
    TestArray<TypeParam, 4> t;
    Matrix<TypeParam> M(2, 2);
    EXPECT_THROW((M << t[0], t[1], t[2], t[3], t[0]), genMatrix::Matrix_Error);
}
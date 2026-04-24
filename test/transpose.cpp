#include "test_main.hpp"

TYPED_TEST(MatrixTest, TransposeSquaredMatrix) {
    using namespace genMatrix;

    TestArray<TypeParam, 4> n;
    Matrix<TypeParam> N(2, 2);
    n.fillmat(N);
    
    Matrix<TypeParam> L = N.transpose();

    Matrix<TypeParam> U = N;
    U.transposeInPlace();

    Matrix<TypeParam> TN(2, 2);
    TN << n[0], n[2], 
            n[1], n[3];

    EXPECT_TRUE(mtx_cmp_test(TN, L));
    EXPECT_TRUE(mtx_cmp_test(TN, U));
}

TYPED_TEST(MatrixTest, TransposeGeneral) {
    using namespace genMatrix;

    TestArray<TypeParam, 6> n;
    Matrix<TypeParam> N(2, 3);
    n.fillmat(N);

    Matrix<TypeParam> L = N.transpose();

    Matrix<TypeParam> U = N;
    U.transposeInPlace();

    Matrix<TypeParam> TN(3, 2);
    TN << n[0], n[3],
            n[1], n[4],
            n[2], n[5];
    
    EXPECT_TRUE(mtx_cmp_test(TN, L));
    EXPECT_TRUE(mtx_cmp_test(TN, U));
}

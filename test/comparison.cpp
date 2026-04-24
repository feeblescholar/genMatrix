#include "test_main.hpp"

TYPED_TEST(MatrixTest, Comparisons) {
    using namespace genMatrix;

    TestArray<TypeParam, 40> t;
    TestArray<TypeParam, 4> t1;
    TestArray<TypeParam, 6> t2;

    Matrix<TypeParam> M(5, 8);
    t.fillmat(M);

    Matrix<TypeParam> M_cp(5, 8);
    t.fillmat(M_cp);

    Matrix<TypeParam> L(2, 2);
    t1.fillmat(L);

    Matrix<TypeParam> A(3, 2);
    t2.fillmat(A);

    EXPECT_EQ(mtx_cmp_test(A, A), A == A);
    EXPECT_EQ(mtx_cmp_test(M, M_cp), M == M_cp);

    EXPECT_EQ(mtx_cmp_test(M_cp, L), M_cp == L);
    EXPECT_EQ(mtx_cmp_test(L, A), L == A);
}
#include "test_main.hpp"

using namespace genMatrix;

TYPED_TEST(MatrixTest, Comparisons) {
    TestArray<TypeParam, 650> t;
    TestArray<TypeParam, 81> t1;
    TestArray<TypeParam, 48> t2;

    Matrix<TypeParam> M(25, 26);
    t.mtx_fill(M);

    Matrix<TypeParam> M_cp(25, 26);
    t.mtx_fill(M_cp);

    Matrix<TypeParam> L(9, 9);
    t1.mtx_fill(L);

    Matrix<TypeParam> A(6, 8);
    t2.mtx_fill(A);

    EXPECT_EQ(mtx_cmp_test(A, A), A == A);
    EXPECT_EQ(mtx_cmp_test(M, M_cp), M == M_cp);

    EXPECT_EQ(mtx_cmp_test(M_cp, L), M_cp == L);
    EXPECT_EQ(mtx_cmp_test(L, A), L == A);
}
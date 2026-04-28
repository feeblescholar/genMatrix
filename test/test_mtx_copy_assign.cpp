#include "test_main.hpp"

using namespace genMatrix;

TYPED_TEST(MatrixTest, CopyConstructor) {
    TestArray<TypeParam, 650> m;
    Matrix<TypeParam> M(25, 26);
    m.mtx_fill(M);

    Matrix<TypeParam> MC = M;
    EXPECT_TRUE(mtx_cmp_test(MC, M));
}

TYPED_TEST(MatrixTest, SelfAssign) {
    TestArray<TypeParam, 650> m;
    Matrix<TypeParam> M(25, 26);
    m.mtx_fill(M);

    Matrix<TypeParam> MC = M;

    /** Ki kell kapcsolni a fordító figyelmeztetését */ 

    #ifdef __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wself-assign"
    #elif defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wself-move"
    #endif

    MC = MC; 

    #ifdef __clang__
        #pragma clang diagnostic pop
    #elif defined(__GNUC__)
        #pragma GCC diagnostic pop
    #endif

    EXPECT_TRUE(mtx_cmp_test(M, MC));
}

TYPED_TEST(MatrixTest, MultiAssign) {
    TestArray<TypeParam, 650> m;
    Matrix<TypeParam> M(25, 26);
    m.mtx_fill(M);

    Matrix<TypeParam> M1 = M;

    TestArray<TypeParam, 49> l;
    Matrix<TypeParam> L(7, 7);
    l.mtx_fill(L);
    
    TestArray<TypeParam, 62> u;
    Matrix<TypeParam> U(1, 62);
    u.mtx_fill(U);

    TestArray<TypeParam, 252> a;
    Matrix<TypeParam> A(21, 12);
    a.mtx_fill(A);

    M1 = L;
    EXPECT_TRUE(mtx_cmp_test(M1, L));

    M1 = U;
    EXPECT_TRUE(mtx_cmp_test(M1, U));

    M1 = A = L;
    EXPECT_TRUE(mtx_cmp_test(M1, A));
    EXPECT_TRUE(mtx_cmp_test(M1, L));
    EXPECT_TRUE(mtx_cmp_test(A, L));
}
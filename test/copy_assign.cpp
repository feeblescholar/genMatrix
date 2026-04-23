#include "test_main.hpp"

TYPED_TEST(MatrixTest, CopyConstructor) {
    using namespace genMatrix;

    TestArray<TypeParam, 24> m;
    Matrix<TypeParam> M(4, 6);
    m.fillmat(M);

    Matrix<TypeParam> M_cp = M;
    EXPECT_TRUE(M_cp == M);
}

TYPED_TEST(MatrixTest, SelfAssign) {
    using namespace genMatrix;

    TestArray<TypeParam, 24> m;
    Matrix<TypeParam> M(4, 6);
    m.fillmat(M);

    Matrix<TypeParam> M_cp = M;

    /** Ki kell kapcsolni a fordító figyelmeztetését, pontosan tudjuk, hogy mit akarunk. */ 

    #ifdef __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wself-assign"
    #elif defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wself-move"
    #endif

    M_cp = M_cp; 

    #ifdef __clang__
        #pragma clang diagnostic pop
    #elif defined(__GNUC__)
        #pragma GCC diagnostic pop
    #endif

    EXPECT_TRUE(M_cp == M);
}

TYPED_TEST(MatrixTest, MultiAssign) {
    using namespace genMatrix;

    TestArray<TypeParam, 24> m;
    Matrix<TypeParam> M(4, 6);
    m.fillmat(M);

    Matrix<TypeParam> M1 = M;

    TestArray<TypeParam, 4> l;
    Matrix<TypeParam> L(2, 2);
    l.fillmat(L);
    
    TestArray<TypeParam, 2> u;
    Matrix<TypeParam> U(1, 2);
    u.fillmat(U);

    TestArray<TypeParam, 6> a;
    Matrix<TypeParam> A(3, 2);
    a.fillmat(A);

    M1 = L;
    EXPECT_TRUE(M1 == L);

    M1 = U;
    EXPECT_TRUE(M1 == U);

    M1 = A = L;
    EXPECT_TRUE(M1 == A);
    EXPECT_TRUE(M1 == L);
    EXPECT_TRUE(A == L);
}
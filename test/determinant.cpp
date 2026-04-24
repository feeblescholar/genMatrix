#include "test_main.hpp"

TYPED_TEST(MatrixTest, Determinant1) {
    using namespace genMatrix;

    Matrix<TypeParam> M1(1, 1);
    TypeParam detM1 = random<TypeParam>();
    M1 << detM1;

    EXPECT_NEAR(detM1, det(M1), 0.0);
}

TYPED_TEST(MatrixTest, Determinant2) {
    using namespace genMatrix;

    TestArray<TypeParam, 4> m;
    Matrix<TypeParam> M2(2, 2);
    m.fillmat(M2);

    TypeParam detM2 = M2(0, 0) * M2(1, 1) - M2(0, 1) * M2(1, 0);

    EXPECT_NEAR(detM2, det(M2), 0.0);
}

TYPED_TEST(MatrixTest, Determinant3) {
    using namespace genMatrix;

    TestArray<TypeParam, 9> m;
    Matrix<TypeParam> M3(3, 3);
    m.fillmat(M3);

    // Sarrus szabály
    TypeParam detM3 = M3(0, 0) * M3(1, 1) * M3(2, 2) + 
                M3(0, 1) * M3(1, 2) * M3(2, 0) + 
                M3(0, 2) * M3(1, 0) * M3(2, 1) - 
                M3(0, 2) * M3(1, 1) * M3(2, 0) - 
                M3(0, 0) * M3(1, 2) * M3(2, 1) - 
                M3(0, 1) * M3(1, 0) * M3(2, 2);

    // kicsit engedünk a pontosságon, a Gauss-elimiáció osztásai miatt, a compiler epsilon túl szigorú nekünk
    EXPECT_NEAR(detM3, det(M3), relaxed_epsilon<TypeParam>());
}

TYPED_TEST(MatrixTest, Determinant4) {
    using namespace genMatrix;

    TestArray<TypeParam, 16> m;
    Matrix<TypeParam> M4(4, 4);
    m.fillmat(M4);

    // ilyen sem lesz többet, innen felfele megbízhatunk az értékekben
    double detM4 =  M4(0,0) * M4(1,1) * M4(2,2) * M4(3,3) + M4(0,0) * M4(1,2) * M4(2,3) * M4(3,1) + M4(0,0) * M4(1,3) * M4(2,1) * M4(3,2) + 
                    M4(0,1) * M4(1,0) * M4(2,3) * M4(3,2) + M4(0,1) * M4(1,2) * M4(2,0) * M4(3,3) + M4(0,1) * M4(1,3) * M4(2,2) * M4(3,0) +
                    M4(0,2) * M4(1,0) * M4(2,1) * M4(3,3) + M4(0,2) * M4(1,1) * M4(2,3) * M4(3,0) + M4(0,2) * M4(1,3) * M4(2,0) * M4(3,1) + 
                    M4(0,3) * M4(1,0) * M4(2,2) * M4(3,1) + M4(0,3) * M4(1,1) * M4(2,0) * M4(3,2) + M4(0,3) * M4(1,2) * M4(2,1) * M4(3,0) - 
                    M4(0,0) * M4(1,1) * M4(2,3) * M4(3,2) - M4(0,0) * M4(1,2) * M4(2,1) * M4(3,3) - M4(0,0) * M4(1,3) * M4(2,2) * M4(3,1) - 
                    M4(0,1) * M4(1,0) * M4(2,2) * M4(3,3) - M4(0,1) * M4(1,2) * M4(2,3) * M4(3,0) - M4(0,1) * M4(1,3) * M4(2,0) * M4(3,2) - 
                    M4(0,2) * M4(1,0) * M4(2,3) * M4(3,1) - M4(0,2) * M4(1,1) * M4(2,0) * M4(3,3) - M4(0,2) * M4(1,3) * M4(2,1) * M4(3,0) - 
                    M4(0,3) * M4(1,0) * M4(2,1) * M4(3,2) - M4(0,3) * M4(1,1) * M4(2,2) * M4(3,0) - M4(0,3) * M4(1,2) * M4(2,0) * M4(3,1);

    // kicsit engedünk a pontosságon, a Gauss-elimiáció osztásai miatt, a compiler epsilon túl szigorú nekünk
    EXPECT_NEAR(detM4, det(M4), relaxed_epsilon<TypeParam>());
}

TYPED_TEST(MatrixTest, DeterminantSingular) {
    using namespace genMatrix;

    Matrix<TypeParam> MS(3, 3);
    MS << 0, 0, 0, 0, 0, 0, 0, 0, 0;

    EXPECT_NEAR((TypeParam)0, det(MS), 0);
}

TYPED_TEST(MatrixTest, NoDeterminant) {
    using namespace genMatrix;

    TestArray<TypeParam, 6> mn;
    Matrix<TypeParam> MN(2, 3);
    mn.fillmat(MN);

    EXPECT_THROW(det(MN), genMatrix::Matrix_Error);
}
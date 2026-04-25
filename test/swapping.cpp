#include "test_main.hpp"
using namespace genMatrix;

TYPED_TEST(MatrixTest, Swapping) {
    TestArray<TypeParam, 12> m1;
    Matrix<TypeParam> M1(4, 3);
    m1.mtx_fill(M1);
    Matrix<TypeParam> M3 = M1;
    
    Matrix<TypeParam> M2(4, 3);
    M2 << M1(1, 0), M1(1, 1), M1(1, 2), 
          M1(0, 0), M1(0, 1), M1(0, 2),
          M1(2, 0), M1(2, 1), M1(2, 2),
          M1(3, 0), M1(3, 1), M3(3, 2);
    Matrix<TypeParam> M5 = M2;

    Matrix<TypeParam> M4(4, 3);
    M4 << M2(0, 2), M2(0, 1), M2(0, 0), 
          M2(1, 2), M2(1, 1), M2(1, 0),
          M2(2, 2), M2(2, 1), M2(2, 0),
          M2(3, 2), M2(3, 1), M2(3, 0);
    
    Matrix<TypeParam> M6(4, 3);
    M6 <<  M1(2, 2), M1(2, 1), M1(2, 0), 
           M1(1, 2), M1(1, 1), M1(1, 0),
           M1(0, 2), M1(0, 1), M1(0, 0),
           M1(3, 2), M1(3, 1), M3(3, 0);

    M1.swapRow(0, 1);
    EXPECT_TRUE(mtx_cmp_test(M1, M2));

    M1.swapRow(0, 1);
    EXPECT_TRUE(mtx_cmp_test(M1, M3));

    EXPECT_THROW(M1.swapRow(-1, 62), genMatrix::Matrix_Error);

    M2.swapCol(0, 2);
    EXPECT_TRUE(mtx_cmp_test(M2, M4));

    M2.swapCol(0, 2);
    EXPECT_TRUE(mtx_cmp_test(M2, M5));

    EXPECT_THROW(M2.swapCol(19, -62), genMatrix::Matrix_Error);

    M1.swapRow(0, 2).swapCol(0, 2);
    EXPECT_TRUE(M1 == M6);
}
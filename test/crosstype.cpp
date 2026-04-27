#include "test_main.hpp"

using namespace genMatrix;

TYPED_TEST(MatrixTest, CrossType_AddSubMul) {
    Matrix<int> mI(5, 5);
    TestArray<int, 25> a1;
    a1.mtx_fill(mI);

    Matrix<float> mF(5, 5);
    TestArray<float, 25> a2;
    a2.mtx_fill(mF);

    Matrix<long> mL(5, 5);
    TestArray<long, 25> a3;
    a3.mtx_fill(mL);


    Matrix<double> mD(5, 5);
    TestArray<double, 25> a4;
    a4.mtx_fill(mD);
    
    Matrix<TypeParam> mR(5, 5);
    TestArray<TypeParam, 25> a5;
    a5.mtx_fill(mR);

    auto res = mI + mD;
    auto res2 = mF - mD;
    auto res3 = mI * mD(0, 0);
    auto res4 = mR + mI;
    auto res5 = mR - mD;
    auto res6 = mR * mF(0, 0);


    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
            EXPECT_TRUE(type_numeric_eq(res(i, j), (mI(i, j) + mD(i, j))));
            EXPECT_TRUE(type_numeric_eq(res2(i, j), (mF(i, j) - mD(i, j))));
            EXPECT_TRUE(type_numeric_eq(res3(i, j), (mI(i, j) * mD(0, 0))));
            EXPECT_TRUE(type_numeric_eq(res4(i, j), (mR(i, j) + mI(i, j))));
            EXPECT_TRUE(type_numeric_eq(res5(i, j), (mR(i, j) - mD(i, j))));
            EXPECT_TRUE(type_numeric_eq(res6(i, j), (mR(i, j) * mF(0, 0))));
        }
    }
}

#include "test_main.hpp"

using namespace genMatrix;

TYPED_TEST(MatrixTest, Norm1) {
    using std::abs;

    TestArray<TypeParam, 6> m;
    Matrix<TypeParam> M(2, 3); 
    m.mtx_fill(M);

    TypeParam col1 = abs(M(0, 0)) + abs(M(1, 0));
    TypeParam col2 = abs(M(0, 1)) + abs(M(1, 1));
    TypeParam col3 = abs(M(0, 2)) + abs(M(1, 2));
    TypeParam norm = std::max(col1, std::max(col2, col3));

    EXPECT_NEAR(norm, norm_1(M), std::numeric_limits<TypeParam>::epsilon() * 10);
}

TYPED_TEST(MatrixTest, NormInf) {
    using std::abs;

    TestArray<TypeParam, 6> m;
    Matrix<TypeParam> M(2, 3); 
    m.mtx_fill(M);

    TypeParam row1 = (abs(M(0, 0)) + abs(M(0, 1)) + abs(M(0, 2)));
    TypeParam row2 = (abs(M(1, 0)) + abs(M(1, 1)) + abs(M(1, 2)));
    TypeParam norm = std::max(row1, row2);

    EXPECT_NEAR(norm, norm_inf(M), std::numeric_limits<TypeParam>::epsilon() * 10);
}

TYPED_TEST(MatrixTest, NormFrobenius) {
    using std::abs;

    TestArray<TypeParam, 650> m;
    Matrix<TypeParam> M(25, 26); 
    m.mtx_fill(M);

    using ResultType = decltype(TypeParam(0) + float(0));
    ResultType sum = 0.0;

    for (size_t i = 0; i < M.getRows(); i++) {
        for (size_t j = 0; j < M.getCols(); j++) {
            sum += M(i, j) * M(i, j);
        }
    }

    EXPECT_NEAR(sqrt(sum), norm_frobenius(M), std::numeric_limits<ResultType>::epsilon() * 1e3);
}
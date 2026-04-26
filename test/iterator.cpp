#include "test_main.hpp"

using namespace genMatrix;

TYPED_TEST(MatrixTest, IteratorTraversal) {
    Matrix<TypeParam> M(2, 3);
    TestArray<TypeParam, 6> m;
    m.mtx_fill(M);

    auto iter = M.begin();
    EXPECT_EQ(*iter, m[0]);
    EXPECT_EQ(*(iter + 1), m[1]);
    EXPECT_EQ(iter + 6, M.end());
}

TYPED_TEST(MatrixTest, IteratorArithmetic) {
    Matrix<TypeParam> M(2, 3);
    TestArray<TypeParam, 6> m;
    m.mtx_fill(M);

    auto iter = M.begin();

    iter += 5;
    EXPECT_EQ(iter - M.begin(), 5);
    
    iter -= 2;
    EXPECT_EQ(*iter, m[3]);
}

TYPED_TEST(MatrixTest, IteratorConstConversion) {
    Matrix<TypeParam> M(2, 2);
    typename Matrix<TypeParam>::iterator iter = M.begin();
    
    typename Matrix<TypeParam>::const_iterator citer = iter; 
    EXPECT_EQ(citer, iter);
}

/** Template horror. Megállapítja, hogy tényleg random_access_iterator Matrix_Iterator. */
TYPED_TEST(MatrixTest, IteratorTraitValidation) {
    EXPECT_TRUE((std::is_base_of_v<std::random_access_iterator_tag, 
                typename std::iterator_traits<typename Matrix<TypeParam>::iterator>::iterator_category>));
}

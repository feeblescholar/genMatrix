#include "test_main.hpp"

using namespace genMatrix;

TYPED_TEST(VectorTest, SanityCheck) {
    Vector<TypeParam> V(2);
    V.push_back(3);
    ASSERT_TRUE(type_numeric_eq((TypeParam)3, V[0]));
}

TYPED_TEST(VectorTest, SizeCalculation) {
    Vector<TypeParam> V(5);

    EXPECT_EQ((size_t)0, V.size());
    EXPECT_EQ((size_t)5, V.capacity());

    V.resize(10);
    EXPECT_EQ((size_t)10, V.capacity());

    V.shrink();
    EXPECT_EQ((size_t)0, V.capacity());

    Vector<TypeParam> V2;

    V2.shrink();
    V2.resize(10);
    EXPECT_EQ((size_t)10, V2.capacity());
}

TYPED_TEST(VectorTest, IndexingCorrect) {
    Vector<TypeParam> V(2);
    TestArray<TypeParam, 2> ta;
    ta.vec_fill(V);

    EXPECT_NO_THROW(EXPECT_TRUE(type_numeric_eq(ta[0], V[0])));
    EXPECT_NO_THROW(EXPECT_TRUE(type_numeric_eq(ta[1], V[1])));
}

TYPED_TEST(VectorTest, IndexingIncorrect) {
    Vector<TypeParam> V(2);
    EXPECT_THROW(V[5], Vector_Error);
    EXPECT_THROW(V[7], Vector_Error);
}
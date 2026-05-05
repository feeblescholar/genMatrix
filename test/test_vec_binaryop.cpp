#include "test_main.hpp"

using namespace genMatrix;

TYPED_TEST(VectorTest, ConstantTimesVector) {
    TestArray<TypeParam, 3> v;
    Vector<TypeParam> V(3);
    v.vec_fill(V);

    Vector<TypeParam> Vexp(4);
    Vexp.push_back(v[0] * 2).push_back(v[1] * 2).push_back(v[2] * 2);

    vec_cmp_test(Vexp, V * 2);
}

TYPED_TEST(VectorTest, AdditionSubtraction) {
    TestArray<TypeParam, 5> v1;
    TestArray<TypeParam, 5> v2;

    Vector<TypeParam> V1(5);
    Vector<TypeParam> V2(5);

    v1.vec_fill(V1);
    v2.vec_fill(V2);

    Vector<TypeParam> addV12 = V1 + V2;
    Vector<TypeParam> subV12 = V1 - V2;
    Vector<TypeParam> subV21 = V2 - V1;

    for (size_t i = 0; i < V1.size(); i++) {
        EXPECT_EQ((TypeParam)(V1[i] + V2[i]), addV12[i]);
        EXPECT_EQ((TypeParam)(V1[i] - V2[i]), subV12[i]);
        EXPECT_EQ((TypeParam)(V2[i] - V1[i]), subV21[i]);
    }
}

TYPED_TEST(VectorTest, Product) {
    TestArray<TypeParam, 3> v1;
    TestArray<TypeParam, 3> v2;

    Vector<TypeParam> V1(3);
    Vector<TypeParam> V2(3);

    v1.vec_fill(V1);
    v2.vec_fill(V2);

    TypeParam exp = V1[0] * V2[0] + V1[1] * V2[1] + V1[2] * V2[2];
    EXPECT_NEAR(exp, (V1 * V2), std::numeric_limits<TypeParam>::epsilon() * 100);
}

TYPED_TEST(VectorTest, InvalidBinaryOperations) {
    Vector<TypeParam> V1(5);
    V1.push_back(2); /** ez kell, mert addig mind a kettő mérete 0 */
    Vector<TypeParam> V2(4);

    EXPECT_THROW((V1 + V2), Vector_Error);
    EXPECT_THROW((V1 - V2), Vector_Error);
    EXPECT_THROW((V1 * V2), Vector_Error);
}
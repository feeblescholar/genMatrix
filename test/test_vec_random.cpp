/**
 * @file test_vec_random.cpp
 * @author Kovács Botond
 * @brief Vektorosztály tesztesetei random értékkel, több típuson.
 */
#include "test_main.hpp"

/**
 * @class VectorTyped
 * @brief Dummy osztály a gtest-hez, a templatek példányosításához szükséges
 */
template<typename T> class VectorTyped : public ::testing::Test {};

/**
 * Itt kell a teszttípusokat megadni.
 */
typedef ::testing::Types<int, long, float, double, genMatrix::Complex> VectorTypes;
TYPED_TEST_SUITE(VectorTyped, VectorTypes);

using namespace genMatrix;

/** Leellenőrizzük, hogy az rng tényleg működik-e és nem csak 1 értéket ad. */
TYPED_TEST(VectorTyped, RNG_SanityCheck) {
    Vector<TypeParam> V(RNG_VEC<TypeParam>(25, lbound, ubound));
    TypeParam randomValue = RNG<TypeParam>(lbound, ubound);

    /** Megjegyzés: 25 + 1 generált elem után nem ér körbe az adott seed. */

    unsigned int match = 0;
    for (size_t i = 0; i < V.size(); i++)
        if (genMatrix::utils::eq<TypeParam>(randomValue, V[i]))
            match++;
    
    /** Ha 5 elem már epszilonon belül van, akkor valami nem jó. */
    ASSERT_LE(match, (unsigned int)5);
}

/** Random számokkal összeadás. */
TYPED_TEST(VectorTyped, Addition) {
    Vector<TypeParam> A(RNG_VEC<TypeParam>(25, lbound, ubound));
    Vector<TypeParam> B(RNG_VEC<TypeParam>(25, lbound, ubound));

    Vector<TypeParam> AB = A + B;
    Vector<TypeParam> BA = B + A;

    EXPECT_TRUE((AB == BA)) << "Not commutative?";

    using EpsilonType = decltype(utils::abs(TypeParam()));

    for (size_t i = 0; i < A.size(); i++)
        CMP_VAL((A[i] + B[i]), AB[i], EPS_L<EpsilonType>);
}

/** Random számokkal kivonás. */
TYPED_TEST(VectorTyped, Subtraction) {
    Vector<TypeParam> A(RNG_VEC<TypeParam>(25, lbound, ubound));
    Vector<TypeParam> B(RNG_VEC<TypeParam>(25, lbound, ubound));

    Vector<TypeParam> AB = A - B;
    Vector<TypeParam> BA = B - A;

    EXPECT_FALSE((AB == BA && !(A == B))) << "A - B and B - A should be different";

    using EpsilonType = decltype(utils::abs(TypeParam()));

    for (size_t i = 0; i < A.size(); i++) {
        CMP_VAL((A[i] - B[i]), AB[i], EPS_L<EpsilonType>);
        CMP_VAL((B[i] - A[i]), BA[i], EPS_L<EpsilonType>);
    }
}

/** Random számokkal belső szorzás. */
TYPED_TEST(VectorTyped, Multiplication) {
    Vector<TypeParam> A(RNG_VEC<TypeParam>(25, lbound, ubound));
    TypeParam C = RNG<TypeParam>(lbound, ubound);

    Vector<TypeParam> CA = A * C; 

    using EpsilonType = decltype(utils::abs(TypeParam()));

    for (size_t i = 0; i < A.size(); i++)
        CMP_VAL(C * A[i], CA[i], EPS_L<EpsilonType>);
}
/**
 * @file test_vec_base.cpp
 * @author Kovács Botond
 * @brief Vektorok tesztesetei ismert értékekkel, valamint egyszerű tesztesetek,
 *        melyek nem igényelnek típusspecifikus tesztelést.
 */
#include "test_main.hpp"

using namespace genMatrix;

/** Minimális elvárható működés tesztelése. */
TYPED_TEST(VectorBase, SanityCheck) {
    Vector<int> V(2);

    ASSERT_THROW(Vector<int> V(0), Vector_Error);

    /** Default méretek. */
    ASSERT_EQ((size_t) 2, V.capacity());
    ASSERT_EQ((size_t) 0, V.size()) << "default constuctor assigned size?";

    /** Push utáni méretek. */
    ASSERT_NO_THROW(V.push_back(1));
    ASSERT_NO_THROW(V.push_back(2));
    ASSERT_EQ((size_t) 2, V.size());
    ASSERT_EQ((size_t) 2, V.capacity());

    /** Kapacitásduplázás. */
    ASSERT_NO_THROW(V.push_back(3)); 
    ASSERT_EQ((size_t) 3, V.size());
    ASSERT_EQ((size_t) 4, V.capacity()) << "push_back should double the capacity";

    /** Megjegyzés: a '<<'-t nem teszteljük, mert a push_back-en alapszik */

    /** Member access. */
    ASSERT_NO_THROW(ASSERT_EQ((int) 1, V[0]));
    ASSERT_NO_THROW(ASSERT_EQ((int) 3, V[2]));
    ASSERT_THROW(V[4], Vector_Error) << "missing bounds check for >= size";
    ASSERT_THROW(V[583], Vector_Error) << "missing bounds check for >= capacity";

    /** Self compare. */
    EXPECT_TRUE(V == V);

    /** Copy konstruktor. */
    Vector<int> VC = V;
    ASSERT_EQ((size_t) 3, VC.size());
    ASSERT_EQ((size_t) 4, VC.capacity());
    ASSERT_NO_THROW(ASSERT_EQ((int) 1, V[0])) << "value changed after copy?";
    ASSERT_NO_THROW(ASSERT_EQ((int) 3, V[2])) << "value changed after copy?";

    /** Copy utáni compare */
    ASSERT_TRUE(V == VC);

}

/** Összeadás ismert értékekkel. */
TYPED_TEST(VectorBase, Addition) {
     Vector<Complex> A(3);
     Vector<Complex> B(3);

     A << Complex(2, 1), Complex(1, -3), Complex(0, -1);
     B << Complex(3, -2), Complex(5, 0), Complex(2, 1);

     Vector<Complex> exp_A_plus_B(3);
     exp_A_plus_B << Complex(5, -1), Complex(6, -3), Complex(2, 0);
     
     CMP_VEC(exp_A_plus_B, A + B, EPS<double>);
     CMP_VEC(exp_A_plus_B, B + A, EPS<double>);
}

/** Kivonás ismert értékekkel. */
TYPED_TEST(VectorBase, Subtraction) {
    Vector<Complex> A(3);
    Vector<Complex> B(3);
    
    A << Complex(2, 1), Complex(1, -3), Complex(0, -1);
    B << Complex(3, -2), Complex(5, 0), Complex(2, 1);

    Vector<Complex> exp_A_minus_B(3);
    exp_A_minus_B << Complex(-1, 3), Complex(-4, -3), Complex(-2, -2);
                   
    CMP_VEC(exp_A_minus_B, A - B, EPS<double>);
}

/** Skaláris szorzás (belső szorzás) ismert értékekkel. */
TYPED_TEST(VectorBase, DotProduct) {
    Vector<Complex> A(2);
    Vector<Complex> B(2);
    
    A << Complex(2, 1), Complex(1, -3);
    B << Complex(3, -2), Complex(5, 0);

    Complex exp_dot_product = Complex(13, -16);

    CMP_VAL(exp_dot_product, (A * B), EPS_L<double>);
}

/** Teszteset az érvénytelen műveletekre. */
TYPED_TEST(VectorBase, InvalidBinaryOperations) {
     Vector<Complex> A(2);
     A << Complex(2, 3), Complex(3, 5);
     Vector<Complex> B(1);
     B << Complex(2, 3);

     EXPECT_THROW(A + B, Vector_Error);
     EXPECT_THROW(B + A, Vector_Error);
     EXPECT_THROW(A - B, Vector_Error);
     EXPECT_THROW(B - A, Vector_Error);
     EXPECT_THROW(A * B, Vector_Error);
     EXPECT_THROW(B * A, Vector_Error);
}
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

    /** Kapacitáscsökkentés */
    V.shrink();
    ASSERT_EQ(V.size(), V.capacity());

    /** Törlések */
    V.pop_back();
    V.remove(0);
    ASSERT_EQ(2, V[0]);

    /** Megjegyzés: a '<<'-t nem teszteljük, mert a push_back-en alapszik */
    V << 1, 3;
    
    /** Member access. */
    ASSERT_NO_THROW(ASSERT_EQ((int) 2, V[0]));
    ASSERT_NO_THROW(ASSERT_EQ((int) 3, V[2]));
    ASSERT_THROW(V[4], Vector_Error) << "missing bounds check for >= size";
    ASSERT_THROW(V[583], Vector_Error) << "missing bounds check for >= capacity";

    /** Self compare. */
    EXPECT_TRUE(V == V);

    /** Copy konstruktor. */
    Vector<int> VC = V;
    ASSERT_EQ((size_t) 3, VC.size());
    ASSERT_EQ((size_t) 3, VC.capacity());
    ASSERT_NO_THROW(ASSERT_EQ((int) 2, V[0])) << "value changed after copy?";
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

TYPED_TEST(VectorBase, IteratorSanityCheck) {
    Vector<long> SRC(8);
    SRC << 1, 2, 3, 4, 5, 6, 7, 8;

    const Matrix<long> V = SRC;
    auto f = V.cbegin();
    auto l = V.cend();

    ASSERT_FALSE(f == l) << "the vector is not empty";

    ASSERT_EQ(1, *f) << "begin() should point to data[0]";
    ASSERT_EQ(8, *(l - 1)) << "end() should point to data[size]";

    ASSERT_EQ(2, *(++f)) << "didn't increment before evaluation";
    ASSERT_EQ(3, *(f++));
    ASSERT_EQ(2, *f) << "previous f++ should not be kept";
    ASSERT_EQ(1, *(--f)) << "didn't decrement before evaluation";

    ASSERT_EQ(5, *(f + 4)) << "indexing is off";

    Vector<Complex> V1(1);
    V1 << Complex(1, 1);

    ASSERT_NEAR(1, V1.begin()->getRe(), EPS_L<double>);
}

TYPED_TEST(VectorBase, STLCompatibility) {
    Vector<long> V(6);
    V <<  92, -21, -30, -4,  -70,  600;

    auto f = V.begin();
    auto l = V.end();

    long neg = std::count_if(f, l, [](long n){ return n < 0; });
    EXPECT_EQ(4, neg);

    std::swap(*(f), *(f + 3));
    EXPECT_EQ(-4, V[0]);

    EXPECT_EQ((long) -70, *std::min_element(f, l));
    EXPECT_EQ((long) 600, *std::max_element(f, l));

    std::sort(f, l);
    EXPECT_TRUE(std::is_sorted(f, l));

    EXPECT_FALSE(std::all_of(f, l, [](long n){ return utils::abs(n) < 100; }));
    EXPECT_TRUE(std::any_of(f, l, [](long n){ return n == -70; }));

    std::fill(f, l, 5);
    EXPECT_TRUE(std::none_of(f, l, [](long n){ return n != 5; }));
}
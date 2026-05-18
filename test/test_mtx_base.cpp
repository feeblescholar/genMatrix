/**
 * @file test_mtx_base.cpp
 * @author Kovács Botond
 * @brief Mátrixok tesztesetei ismert értékekkel, valamint egyszerű tesztesetek,
 *        melyek nem igényelnek típusspecifikus tesztelést.
 */
#include "test_main.hpp"

using namespace genMatrix;

/** Minimális elvárható működés tesztelése. A többi teszt ennyit feltételez. */
TYPED_TEST(MatrixBase, SanityCheck) {
    Matrix<int> M(2, 3);

    ASSERT_EQ((size_t) 2, M.getRows());
    ASSERT_EQ((size_t) 3, M.getCols());
    ASSERT_EQ((size_t) 6, M.size());

    ASSERT_NO_THROW((M << 1, 2, 3, 4, 5, 6)) << "CommaInit counter?";

    ASSERT_EQ((int) 3, M(0, 2)) << "indexing is off?";
    ASSERT_EQ((int) 6, M(1, 2)) << "indexing is off?";
    
    ASSERT_THROW(M(2, 3), Matrix_Error) << "OoB index check?";
    ASSERT_THROW(M.resize(2, 2), Matrix_Error) << "Resize on static?";

    Matrix<double> MD;

    ASSERT_NO_THROW(MD.resize(2, 4));
    ASSERT_THROW((MD << 1, 2), Matrix_Error) << "CommaInit counter?";
    
    ASSERT_TRUE(M == M);
    ASSERT_FALSE(M == MD);

    Matrix<int> M1 = M;

    ASSERT_EQ(M(0, 0), M1(0, 0)) << "stored value changed after copy?";
    ASSERT_EQ(M(1, 2), M1(1, 2)) << "stored value changed after copy?";

    ASSERT_TRUE(M1 == M);
}

/** Összeadás ismert értékekkel. */
TYPED_TEST(MatrixBase, Addition) {
     Matrix<Complex> A(2, 2);
     Matrix<Complex> B(2, 2);

     A << Complex(2, 1), Complex(1, -3),
          Complex(0, -1), Complex(4, 2);

     B << Complex(3, -2), Complex(5, 0),
          Complex(2, 1), Complex(0, -1);

     Matrix<Complex> exp_A_plus_B(2, 2);
     exp_A_plus_B << Complex(5, -1), Complex(6, -3),
                     Complex(2, 0),  Complex(4, 1);
     
     CMP_MTX(exp_A_plus_B, A + B, EPS<double>);
     CMP_MTX(exp_A_plus_B, B + A, EPS<double>);
}

/** Kivonás ismert értékekkel. */
TYPED_TEST(MatrixBase, Subtraction) {
    Matrix<Complex> A(2, 2);
    Matrix<Complex> B(2, 2);
    
    A << Complex(2, 1), Complex(1, -3),
         Complex(0, -1), Complex(4, 2);

    B << Complex(3, -2), Complex(5, 0),
         Complex(2, 1), Complex(0, -1);

    Matrix<Complex> exp_A_minus_B(2, 2);
    exp_A_minus_B << Complex(-1, 3),  Complex(-4, -3),
                     Complex(-2, -2), Complex(4, 3);
                   
    CMP_MTX(exp_A_minus_B, A - B, EPS<double>);
}

/** Szorzás ismert értékekkel. */
TYPED_TEST(MatrixBase, Multiplication) {
    Matrix<Complex> A(2, 2);
    Matrix<Complex> B(2, 2);
    
    A << Complex(2, 1), Complex(1, -3),
         Complex(0, -1), Complex(4, 2);

    B << Complex(3, -2), Complex(5, 0),
         Complex(2, 1), Complex(0, -1);
    
    Matrix<Complex> exp_A_mul_B(2, 2);
    exp_A_mul_B << Complex(13, -6), Complex(7, 4),
                   Complex(4, 5),   Complex(2, -9);

    CMP_MTX(exp_A_mul_B, A * B, EPS<double>);
}

/** Teszteset az érvénytelen műveletekre. */
TYPED_TEST(MatrixBase, InvalidBinaryOperations) {
     Matrix<Complex> A(2, 3);
     Matrix<Complex> B(7, 9);

     EXPECT_THROW(A + B, Matrix_Error);
     EXPECT_THROW(B + A, Matrix_Error);
     EXPECT_THROW(A - B, Matrix_Error);
     EXPECT_THROW(B - A, Matrix_Error);
     EXPECT_THROW(A * B, Matrix_Error);
     EXPECT_THROW(B * A, Matrix_Error);
}

/** Transzponálás. */
TYPED_TEST(MatrixBase, Transpose) {
     Matrix<double> M(2, 3);
     M << 1, 2, 3, 
          4, 5, 6;

     Matrix<double> MT = M.transpose();
     M.transposeInPlace();

     Matrix<double> exp(3, 2);
     exp << 1, 4,
            2, 5,
            3, 6;

     CMP_MTX(exp, MT, EPS_L<double>);
     CMP_MTX(exp, M, EPS_L<double>);
}

/*÷ Determináns számítás ismert értékekkel. */
TYPED_TEST(MatrixBase, Determinant) {
    Matrix<double> A(2, 2);
    Matrix<Complex> B(3, 3);

    A << 4.5, 2.0,
         1.5, 3.0;

    B << Complex(1, 1),  Complex(0, 0),  Complex(2, 0),
         Complex(0, -1), Complex(1, 0),  Complex(0, 1),
         Complex(3, 0),  Complex(0, 2),  Complex(1, -1);

    double exp_detA = 10.5;
    Complex exp_detB = Complex(2, 2);

    CMP_VAL(exp_detA, det(A), EPS<double>);
    CMP_VAL(exp_detB, det(B), EPS_L<double>);
}

/** 0-ás determináns teszt */
TYPED_TEST(MatrixBase, DeterminantSingular) {
    Matrix<double> MS(3, 3);
    MS << 0, 0, 0, 0, 0, 0, 0, 0, 0;
    CMP_VAL(0.0, det(MS), 0.0);
}

/** Inverz számítás ismert értékekkel. */
TYPED_TEST(MatrixBase, Inverse) {
    Matrix<double> A(3, 3);
    Matrix<Complex> B(3, 3);

    A << 1, 0, 1,
         2, 1, 0,
         0, 1, 1;

    B << Complex(1, 0), Complex(0, 0), Complex(0, 1),
         Complex(0, 0), Complex(1, 0), Complex(0, 0),
         Complex(2, 0), Complex(0, 0), Complex(1, 0);

    Matrix<double> exp_inv_A(3, 3);
    Matrix<Complex> exp_inv_B(3, 3);
    
    exp_inv_A << (1.0 / 3.0),  (1.0 / 3.0), (-1.0 / 3.0),
                 (-2.0 / 3.0), (1.0 / 3.0),  (2.0 / 3.0),
                 (2.0 / 3.0),  (-1.0 / 3.0), (1.0 / 3.0);

    exp_inv_B << Complex(0.2, 0.4),   Complex(0.0, 0.0), Complex(0.4, -0.2),
                 Complex(0.0, 0.0),   Complex(1.0, 0.0), Complex(0.0, 0.0),
                 Complex(-0.4, -0.8), Complex(0.0, 0.0), Complex(0.2, 0.4);

    CMP_MTX(exp_inv_A, A.inverse(), EPS_L<double>);
    CMP_MTX(exp_inv_B, B.inverse(), EPS_L<double>);
}

/** Nem négyzetes mátrix determinánsa/inverze nem létezik. */
TYPED_TEST(MatrixBase, InvalidDeterminantInverse) {
    Matrix<int> M(2, 3);
    EXPECT_THROW(det(M), Matrix_Error);
    EXPECT_THROW(M.inverse(), Matrix_Error);
}

/** Mátrixok normáinak tesztelése. */
TYPED_TEST(MatrixBase, DiscreteNorm) {
     Matrix<double> A(3, 3);
     Matrix<Complex> B(3, 3);

     A << 1, 0, 1,
          2, 7, 0,
          0, 1, 1;

     B << Complex(1, 0), Complex(0, 0), Complex(0, 7),
          Complex(0, 0), Complex(1, 0), Complex(0, 0),
          Complex(2, 0), Complex(4, 1), Complex(1, 0);

     double exp_A_norm_1 = 8;
     double exp_A_norm_inf = 9;
     double exp_A_norm_f = 7.54983443527075;

     double exp_B_norm_1 = 8;
     double exp_B_norm_inf = 8;
     double exp_B_norm_f = 8.54400374531753;

     CMP_VAL(exp_A_norm_1, genMatrix::norm_1(A), EPS_L<double>);
     CMP_VAL(exp_A_norm_inf, genMatrix::norm_inf(A), EPS_L<double>);
     CMP_VAL(exp_A_norm_f, genMatrix::norm_frobenius(A), EPS_L<double>);
     CMP_VAL(exp_B_norm_1, genMatrix::norm_1(B), EPS_L<double>);
     CMP_VAL(exp_B_norm_inf, genMatrix::norm_inf(B), EPS_L<double>);
     CMP_VAL(exp_B_norm_f, genMatrix::norm_frobenius(B), EPS_L<double>);
}

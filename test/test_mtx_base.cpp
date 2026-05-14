#include "test_main.hpp"

#ifdef new
#undef new
#undef delete
#endif

/** Minimális elvárható működés tesztelése. */
TEST(MatrixBase, SanityCheck) {
    MTX<int> M(2, 3);

    ASSERT_EQ((size_t) 2, M.getRows());
    ASSERT_EQ((size_t) 3, M.getCols());
    ASSERT_EQ((size_t) 6, M.size());

    ASSERT_NO_THROW((M << 1, 2, 3, 4, 5, 6));

    ASSERT_EQ((int) 3, M(0, 2));
    ASSERT_EQ((int) 6, M(1, 2));
    
    ASSERT_THROW(M(2, 3), genMatrix::Matrix_Error);
    ASSERT_THROW(M.resize(2, 2), genMatrix::Matrix_Error);

    MTX<double> MD;

    ASSERT_NO_THROW(MD.resize(2, 4));
    ASSERT_THROW((MD << 1, 2), genMatrix::Matrix_Error);
    
    ASSERT_TRUE(M == M);
    ASSERT_FALSE(M == MD);
}

/** Összeadás, kivonás szorzás ismert értékekkel. */
TEST(MatrixBase, DiscreteBinaryOperations) {
    MTX<CX> A(2, 2);
    MTX<CX> B(2, 2);
    
    A << CX(2, 1), CX(1, -3),
         CX(0, -1), CX(4, 2);

    B << CX(3, -2), CX(5, 0),
         CX(2, 1), CX(0, -1);

    MTX<CX> exp_A_plus_B(2, 2);
    exp_A_plus_B << CX(5, -1), CX(6, -3),
                    CX(2, 0),  CX(4, 1);

    MTX<CX> exp_A_minus_B(2, 2);
    exp_A_minus_B << CX(-1, 3),  CX(-4, -3),
                     CX(-2, -2), CX(4, 3);
    
    MTX<CX> exp_A_mul_B(2, 2);
    exp_A_mul_B << CX(13, -6), CX(7, 4),
                   CX(4, 5),   CX(2, -9);

    CMP_MTX(exp_A_plus_B, A + B, EPS<double>);
    CMP_MTX(exp_A_minus_B, A - B, EPS<double>);
    CMP_MTX(exp_A_mul_B, A * B, EPS<double>);
}

/* Determináns számítás ismert értékekkel. */
TEST(MatrixBase, DiscreteDeterminant) {
    MTX<double> A(2, 2);
    MTX<CX> B(3, 3);

    A << 4.5, 2.0,
         1.5, 3.0;

    B << CX(1, 1),  CX(0, 0),  CX(2, 0),
         CX(0, -1), CX(1, 0),  CX(0, 1),
         CX(3, 0),  CX(0, 2),  CX(1, -1);

    double exp_detA = 10.5;
    CX exp_detB = CX(2, 2);

    CMP_VAL(exp_detA, det(A), EPS<double>);
    CMP_VAL(exp_detB, det(B), EPS_L<double>);
}

/* 0-ás determináns teszt */
TEST(MatrixTest, DeterminantSingular) {
    MTX<double> MS(3, 3);
    MS << 0, 0, 0, 0, 0, 0, 0, 0, 0;
    CMP_VAL(0.0, det(MS), 0.0);
}

/* Inverz számítás ismert értékekkel. */
TEST(MatrixBase, DiscreteInverse) {
    MTX<double> A(3, 3);
    MTX<CX> B(3, 3);

    A << 1, 0, 1,
         2, 1, 0,
         0, 1, 1;

    B << CX(1, 0), CX(0, 0), CX(0, 1),
         CX(0, 0), CX(1, 0), CX(0, 0),
         CX(2, 0), CX(0, 0), CX(1, 0);

    MTX<double> exp_inv_A(3, 3);
    MTX<CX> exp_inv_B(3, 3);
    
    exp_inv_A << (1.0 / 3.0),  (1.0 / 3.0), (-1.0 / 3.0),
                 (-2.0 / 3.0), (1.0 / 3.0),  (2.0 / 3.0),
                 (2.0 / 3.0),  (-1.0 / 3.0), (1.0 / 3.0);

    exp_inv_B << CX(0.2, 0.4),   CX(0.0, 0.0), CX(0.4, -0.2),
                 CX(0.0, 0.0),   CX(1.0, 0.0), CX(0.0, 0.0),
                 CX(-0.4, -0.8), CX(0.0, 0.0), CX(0.2, 0.4);

    CMP_MTX(exp_inv_A, A.inverse(), EPS_L<double>);
    CMP_MTX(exp_inv_B, B.inverse(), EPS_L<double>);
}

/* Nem négyzetes mátrix determinánsa/inverze nem létezik. */
TEST(MatrixBase, NoDetInv) {
    MTX<int> M(2, 3);
    EXPECT_THROW(det(M), genMatrix::Matrix_Error);
    EXPECT_THROW(det(M), genMatrix::Matrix_Error);
}

TEST(MatrixBase, DiscreteNorm) {
     MTX<double> A(3, 3);
     MTX<CX> B(3, 3);

     A << 1, 0, 1,
          2, 7, 0,
          0, 1, 1;

     B << CX(1, 0), CX(0, 0), CX(0, 7),
          CX(0, 0), CX(1, 0), CX(0, 0),
          CX(2, 0), CX(4, 1), CX(1, 0);

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

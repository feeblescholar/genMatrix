#include "test_main.hpp"

/**
 * @class MatrixTest
 * @brief Dummy osztály a gtest-hez, a templatek példányosításához szükséges
 */
template<typename T> class MatrixTest : public ::testing::Test {};

/**
 * Itt kell a teszttípusokat megadni.
 */
typedef ::testing::Types<int, long, float, double, genMatrix::Complex> MatrixTypes;
TYPED_TEST_SUITE(MatrixTest, MatrixTypes);


#include "test_main.hpp"

using namespace genMatrix;

const int lbound = -12;
const int ubound = 20;

TYPED_TEST(MatrixTest, Determinant2) {
    Matrix<TypeParam> M(RNG_MTX<TypeParam>(2, 2, lbound, ubound));
    TypeParam exp_det = M(0, 0) * M(1, 1) - M(0, 1) * M(1, 0);

    using epsilonType = decltype(utils::abs(TypeParam()));
    CMP_VAL(exp_det, det(M), EPS<epsilonType> * M.size());
}

TYPED_TEST(MatrixTest, Determinant3) {
    Matrix<TypeParam> M(RNG_MTX<TypeParam>(3, 3, lbound, ubound));

    /** Sarrus szabály alapján */
    TypeParam exp_det = M(0, 0) * M(1, 1) * M(2, 2) + 
                        M(0, 1) * M(1, 2) * M(2, 0) + 
                        M(0, 2) * M(1, 0) * M(2, 1) - 
                        M(0, 2) * M(1, 1) * M(2, 0) - 
                        M(0, 0) * M(1, 2) * M(2, 1) - 
                        M(0, 1) * M(1, 0) * M(2, 2);

    
    using epsilonType = decltype(utils::abs(TypeParam()));
    /** kicsit engedünk a pontosságon, a Gauss-elimiáció osztásai miatt */
    CMP_VAL(exp_det, det(M), EPS_L<epsilonType> * M.size());
}
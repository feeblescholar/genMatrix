#include "test_main.hpp"

/**
 * @class MatrixTyped
 * @brief Dummy osztály a gtest-hez, a templatek példányosításához szükséges
 */
template<typename T> class MatrixTyped : public ::testing::Test {};

/**
 * Itt kell a teszttípusokat megadni.
 */
typedef ::testing::Types<int, long, float, double, genMatrix::Complex> MatrixTypes;
TYPED_TEST_SUITE(MatrixTyped, MatrixTypes);


#include "test_main.hpp"

using namespace genMatrix;

const int lbound = -12;
const int ubound = 20;

TYPED_TEST(MatrixTyped, Addition) {
    Matrix<TypeParam> A(RNG_MTX<TypeParam>(5, 5, lbound, ubound));
    Matrix<TypeParam> B(RNG_MTX<TypeParam>(5, 5, lbound, ubound));

    Matrix<TypeParam> AB = A + B;
    Matrix<TypeParam> BA = B + A;

    EXPECT_TRUE((AB == BA)) << "Not commutative?";

    using EpsilonType = decltype(utils::abs(TypeParam()));

    for (size_t i = 0; i < A.getRows(); i++)
        for (size_t j = 0; j < A.getCols(); j++)
            CMP_VAL((A(i, j) + B(i, j)), AB(i, j), EPS_L<EpsilonType>);
}

TYPED_TEST(MatrixTyped, Subtraction) {
    Matrix<TypeParam> A(RNG_MTX<TypeParam>(5, 5, lbound, ubound));
    Matrix<TypeParam> B(RNG_MTX<TypeParam>(5, 5, lbound, ubound));

    Matrix<TypeParam> AB = A - B;
    Matrix<TypeParam> BA = B - A;

    EXPECT_FALSE((AB == BA && !(A == B))) << "A - B and B - A should be different";

    using EpsilonType = decltype(utils::abs(TypeParam()));

    for (size_t i = 0; i < A.getRows(); i++) {
        for (size_t j = 0; j < A.getCols(); j++) {
            CMP_VAL((A(i, j) - B(i, j)), AB(i, j), EPS_L<EpsilonType>);
            CMP_VAL((B(i, j) - A(i, j)), BA(i, j), EPS_L<EpsilonType>);
        }
    }
}

TYPED_TEST(MatrixTyped, Multiplication) {
    Matrix<TypeParam> A(RNG_MTX<TypeParam>(2, 3, lbound, ubound));
    Matrix<TypeParam> B(RNG_MTX<TypeParam>(3, 4, lbound, ubound));


    Matrix<TypeParam> AB(2, 4);
    /** egyszer és utoljára végigmásszuk kézzel */
    AB <<  A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0),
           A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1),
           A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2),
           A(0, 0) * B(0, 3) + A(0, 1) * B(1, 3) + A(0, 2) * B(2, 3),
           A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0),
           A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1),
           A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2),
           A(1, 0) * B(0, 3) + A(1, 1) * B(1, 3) + A(1, 2) * B(2, 3);

    using EpsilonType = decltype(utils::abs(TypeParam()));
    CMP_MTX(AB, (A * B), EPS_L<EpsilonType>);

    TypeParam C = RNG<TypeParam>(lbound, ubound);

    /** a komplex szereti itt a sajátját is */
    Matrix<TypeParam> CA = A.operator*(C); 

    for (size_t i = 0; i < A.getRows(); i++)
        for (size_t j = 0; j < A.getCols(); j++)
            CMP_VAL(C * A(i, j), CA(i, j), EPS_L<EpsilonType>);

}

TYPED_TEST(MatrixTyped, Determinant_2x2) {
    Matrix<TypeParam> M(RNG_MTX<TypeParam>(2, 2, lbound, ubound));
    TypeParam exp_det = M(0, 0) * M(1, 1) - M(0, 1) * M(1, 0);

    using EpsilonType = decltype(utils::abs(TypeParam()));
    CMP_VAL(exp_det, det(M), EPS_L<EpsilonType> * M.size());
}

TYPED_TEST(MatrixTyped, Determinant_3x3) {
    Matrix<TypeParam> M(RNG_MTX<TypeParam>(3, 3, lbound, ubound));

    /** Sarrus szabály alapján */
    TypeParam exp_det = M(0, 0) * M(1, 1) * M(2, 2) + 
                        M(0, 1) * M(1, 2) * M(2, 0) + 
                        M(0, 2) * M(1, 0) * M(2, 1) - 
                        M(0, 2) * M(1, 1) * M(2, 0) - 
                        M(0, 0) * M(1, 2) * M(2, 1) - 
                        M(0, 1) * M(1, 0) * M(2, 2);

    
    using EpsilonType = decltype(utils::abs(TypeParam()));
    /** kicsit engedünk a pontosságon, a Gauss-elimiáció osztásai miatt */
    CMP_VAL(exp_det, det(M), EPS_L<EpsilonType> * 1e3);
}

TYPED_TEST(MatrixTyped, Inverse_5x5) {
    Matrix<TypeParam> M(RNG_MTX<TypeParam>(5, 5, lbound, ubound));

    /** ki kell találni milyen típust fog az inverz dobni */
    using ReturnType = decltype(TypeParam() + double());
    using EpsilonType = decltype(utils::abs(ReturnType()));
    
    Matrix<ReturnType> I(5, 5);
    for (size_t i = 0; i < I.getRows(); i++)
        I(i, i) = TypeParam(1);
    
    
    CMP_MTX(I, (M * M.inverse()), EPS_L<EpsilonType>);
}
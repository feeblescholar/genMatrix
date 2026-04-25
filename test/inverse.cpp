#include "test_main.hpp"
#include "test_utils.hpp"

using namespace genMatrix;

TYPED_TEST(MatrixTest, Inverse) {
    Matrix<TypeParam> M(1000, 1000);
    TestArray<TypeParam, 1000000> m;
    m.fillmat(M);

    /** ki kell találni az új típust */
    using ExpT = decltype(TypeParam(0), double(0));

    /** itt nem a típusközti szorzás teszt a légyeg, átalakítjuk a base-t is. */
    Matrix<ExpT> M1 = M;
    Matrix<ExpT> M1inv = M1.inverse();

    Matrix<ExpT> identityMatrix(1000, 1000);
    for (size_t i = 0; i < identityMatrix.getRows(); i++) {
        identityMatrix(i, i) = 1;
    }

    /** kihasználjuk azt a tételt, miszerint: M * inv(M) = inv(M) * M = E */
    EXPECT_TRUE(mtx_cmp_test((M1 * M1inv), identityMatrix, relaxed_epsilon<ExpT>()));
    EXPECT_TRUE(mtx_cmp_test((M1inv * M1), identityMatrix, relaxed_epsilon<ExpT>()));
}
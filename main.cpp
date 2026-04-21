#include "test_static.hpp"

int main(int argc, char **argv) {
    using namespace genMatrix;

    genMatrixTest::full_genMatrix_test__static<int>();
    genMatrixTest::full_genMatrix_test__static<double>();
    genMatrixTest::full_genMatrix_test__static<float>();
    genMatrixTest::full_genMatrix_test__static<long>();

    return 0;
}
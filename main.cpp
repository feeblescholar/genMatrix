#include "test_static.hpp"

int main(int argc, char **argv) {
    using namespace genMatrix;

    //genMatrixTest::genMatrix_static_test<int>();
    genMatrixTest::genMatrix_static_test<double>(0);
    //genMatrixTest::genMatrix_static_test<float>();
    //genMatrixTest::genMatrix_static_test<long>();

    return 0;
}
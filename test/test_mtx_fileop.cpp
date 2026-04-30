#include "test_main.hpp"

#include <fstream>
#include <filesystem>

using namespace genMatrix;
namespace fs = std::filesystem;

TYPED_TEST(MatrixTest, FileRW) {
    Matrix<TypeParam> M(2, 3);
    TestArray<TypeParam, 6> m;
    m.mtx_fill(M);
    
    fs::path p = fs::current_path() / "build/test.txt";

    std::ofstream ofile(p, std::ofstream::out | std::ofstream::trunc);
    if (ofile.is_open())
        ofile << M << std::flush;
    else
        FAIL() << "nem lehet fajlt nyitni";
    ofile.close();

    std::ifstream ifile(p, std::ifstream::in);
    Matrix<TypeParam> MR;
    if (ifile.is_open())
        ifile >> MR;
    else
        FAIL() << "nem lehet fajlt nyitni";
    ifile.close();

    EXPECT_TRUE(mtx_cmp_test(M, MR));
}
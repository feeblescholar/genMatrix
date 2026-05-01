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

TYPED_TEST(MatrixTest, FileRWMultiple) {
    Matrix<TypeParam> M(21, 32);
    Matrix<TypeParam> M1(20, 23);
    Matrix<TypeParam> M2(24, 27);

    TestArray<TypeParam, 21 * 32> m;
    TestArray<TypeParam, 20 * 23> m1;
    TestArray<TypeParam, 24 * 27> m2;

    m.mtx_fill(M);
    m1.mtx_fill(M1);
    m2.mtx_fill(M2);
    
    fs::path p = fs::current_path() / "build/test.txt";

    std::ofstream ofile(p, std::ofstream::out | std::ofstream::trunc);
    if (ofile.is_open()) {
        ofile << M << std::flush;
        ofile << M1 << std::flush;
        ofile << M2 << std::flush;
    }     
    else
        FAIL() << "nem lehet fajlt nyitni";
     ofile.close();

    std::ifstream ifile(p, std::ifstream::in);
    Matrix<TypeParam> MR;
    Matrix<TypeParam> MR1;
    Matrix<TypeParam> MR2;

    if (ifile.is_open()) {
        ifile >> MR;
        ifile >> MR1;
        ifile >> MR2;
    }   
    else
        FAIL() << "nem lehet fajlt nyitni";
    ifile.close();


    EXPECT_TRUE(mtx_cmp_test(M, MR));
    EXPECT_TRUE(mtx_cmp_test(M1, MR1));
    EXPECT_TRUE(mtx_cmp_test(M2, MR2));
}
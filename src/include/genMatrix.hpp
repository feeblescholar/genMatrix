/**
 * @file genMatrix.hpp
 * @author Kovács Botond
 * @brief Main header file a genMatrixhoz.
 */
#ifndef GENMATRIX
#if __cplusplus >= 201703L
    #define GENMATRIX

    #include "utils.hpp"

    #include "matrix.hpp"
    #include "matrix_commaInit.hpp"
    #include "matrix_error.hpp"

    #include "determinant.hpp"
    #include "norms.hpp"
    #include "fhandler.hpp"

    #include "vector.hpp"
    #include "complex.hpp"
#else
    #error "This library requires C++17 or never";
#endif
#endif
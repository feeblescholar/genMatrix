/**
 * @file fhandler.hpp
 * @author Kovács Botond
 * @brief iostream operátorok a genMatrixhoz.
 */
#ifndef FHANDLER_H
#define FHANDLER_H

#include <iostream>
#include <iomanip>
#include <string>

#include "matrix.hpp"

/** Olvasó buffer mérete. */
const size_t READ_BUFFER_SIZE = 1024;

/**
 * @brief Kiírja a kapott mátrixot az output streamre.
 * @param os Az output stream referenciája.
 * @param mtx A kiírandó mátrix referenciája.
 * @return A kapott output stream.
 */
template<typename T>
std::ostream& operator<<(std::ostream& os, const genMatrix::Matrix<T>& mtx) {
    os << "[genMatrix ostream output begin]" << std::endl
       << "[size -> rows] " << mtx.getRows() << std::endl
       << "[size -> cols] " << mtx.getCols() << std::endl;


    std::streamsize prev_pr = os.precision();
    os << std::setprecision(std::numeric_limits<T>::max_digits10);
    
    for (size_t i = 0; i < mtx.getRows(); i++) {
        for (size_t j = 0; j < mtx.getCols(); j++)
            os << mtx(i, j) << " ";
        os << std::endl;
    }

    os << std::setprecision(prev_pr);
    os << "[genMatrix ostream output end]" << std::endl;
    return os;
}

/**
 * @brief Feltölt egy mátrixot a kapott input streamről.
 * @param is Az input stream referenciája.
 * @param mtx A feltöltendő mátrix referenciája.
 * @return A kapott input stream.
 */
template<typename T>
std::istream& operator>>(std::istream& is, genMatrix::Matrix<T>& mtx) {
    /* A feladat (ld. doc/Specifikáció.md) tiltja az STL tárolót azért nem string. */
    char buf[READ_BUFFER_SIZE];
    size_t rows = 0, cols = 0;

    is.getline(buf, READ_BUFFER_SIZE);
    is >> buf >> buf >> buf >> rows;
    is >> buf >> buf >> buf >> cols;
    mtx.resize(rows, cols); 

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            is >> mtx(i, j);
        }
    }

    is >> buf;
    is.getline(buf, READ_BUFFER_SIZE);
    return is;
}

#endif

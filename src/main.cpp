#include "include/genMatrix.hpp"

#include <iostream>

int main(void) {
    /**
     * specben lévő teszt
     */

    /**
     * Oldjuk meg a következő egyenletrendszert!
     * 2x +  y -  z + 3w +  v = 7
     * -x + 4y + 2z -  w + 5v = 3
     * 3x - 2y +  z + 2w -  v = 10
     * x +  y +  z +  w +  v = 8
     * 5x - 3y + 4z - 2w + 2v = 1
     */

     /** A együtthatómátrix feltöltése */
     genMatrix::Matrix<int> A(5, 5);
     A <<  2,  1, -1,  3,  1,
          -1,  4,  2, -1,  5,
           3, -2,  1,  2, -1,
           1,  1,  1,  1,  1,
           5, -3,  4, -2,  2;
    
    /** B oszlopvektor feltöltése (itt most egy 5 soros 1 oszlopos mátrix) */
    genMatrix::Matrix<int> B(5, 1);
    B << 7, 3, 10, 8, 1;

    /** Ainv * B a megoldás */
    auto Ainv = A.inverse();
    auto sol = Ainv * B;

    /** A mátrixnak van iterátora, mehet range based */
    for (auto item : sol) {
        std::cout << item << " "; 
    }
    return 0;
}
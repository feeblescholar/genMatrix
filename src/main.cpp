#include "include/genMatrix.hpp"
#include "../lib/memtrace_gtest/gtest_lite.h"

#include <iostream>

using namespace genMatrix;

/** 
 * Hibajavítás miatt (commit 788b2185adfd130e573b3fe3d42e59d4a4d34c37):
 * genMatrix CI/[prod][windows-latest][x86]: M_PI was not declared in this scope
 */
#define PI 3.14159265358979323846264338327950288

int main(void) {
    /**
     * Oldjuk meg a következő egyenletrendszert!
     * 
     * 2x +  y -  z + 3w +  v = 7
     * -x + 4y + 2z -  w + 5v = 3
     * 3x - 2y +  z + 2w -  v = 10
     * x +  y +  z +  w +  v = 8
     * 5x - 3y + 4z - 2w + 2v = 1
     */
    TEST(genMatrixExamples, LinearSystemOfEquations)
        Matrix<int> A(5, 5);
        A <<  2,  1, -1,  3,  1,
             -1,  4,  2, -1,  5,
              3, -2,  1,  2, -1,
              1,  1,  1,  1,  1,
              5, -3,  4, -2,  2;

        Vector<int> B(5);
        B << 7, 3, 10, 8, 1;

        /** Ainv * B a megoldás */
        auto Ainv = A.inverse();
        auto sol = Ainv * (Matrix<int>)B;

        /** Az elvárt megoldás */
        double x = 0.08045977011494253;
        double y = 1.793103448275862;
        double z = 4.218390804597701;
        double w = 3.67816091954023;
        double v = -1.7701149425287357;

        EXPECT_DOUBLE_EQ(x, sol(0, 0));
        EXPECT_DOUBLE_EQ(y, sol(1, 0));
        EXPECT_DOUBLE_EQ(z, sol(2, 0));
        EXPECT_DOUBLE_EQ(w, sol(3, 0));
        EXPECT_DOUBLE_EQ(v, sol(4, 0));
    END

    /**
     * Weboldalak rangsorolása Markov-lánccal (PageRank)
     * 
     * A feladat egy egyszerűsített Google PageRank algoritmus megvalósítása a 
     * mátrixkönyvtár tesztelésére. A teszt itt az alábbi P weboldalátmenet-valószínűséggel 
     * dolgozik és V-ben számítja ki az egyes oldallátogatás valószínűszínűségét.
     */
    TEST(genMatrixExamples, StationaryDistribution)
        Matrix<double> P(3, 3);
        P << 0.25, 0.5,  0.5, 
             0.5,  0.25, 0.25, 
             0.25, 0.25, 0.25;

        Vector<double> V;
        V << 0.3, 0.3, 0.4; 

        double eps = 1e-14;

        for (int i = 0; i < 100; i++) {
            Vector<double> Vn = P * V; 
            Vector<double> diff = Vn - V; 

            if (norm_inf((Matrix<double>) diff) < eps) {
                V = Vn;
                break;
            }

            V = Vn; 
        }

        EXPECT_DOUBLE_EQ(0.4, V[0]);
        EXPECT_DOUBLE_EQ(0.35, V[1]);
        EXPECT_DOUBLE_EQ(0.25, V[2]);
    END

    /**
     * 3D-s ponttranszformáció
     * 
     * Ebben a példában egy 3D-s pontot (P) fogunk transzformálni.
     * Mondjuk ezt a GPU szokta végezni, de itt most a CPU-t kínozzuk meg vele.
     * Ehhez szükségünk van egy tolás (shift), forgatás (rot) és egy skálázó (scale) 
     * mátrixra. Ebben a példában P-t elmozgatjuk 2.5 egységgel jobbra és 1.0 egységgel 
     * lefele, megforgatjuk a Z tengely körül 45 fokkal és felskálázzuk a kétszeresére.
     */
    TEST(genMatrixExamples, Transformation3D)
        Matrix<double> shift(4, 4);
        Matrix<double> rot(4, 4);
        Matrix<double> scale(4, 4);
        Vector<double> P;
        double a = 45.0 * PI / 180.0;

        shift << 1, 0, 0, 2.5,
                0, 1, 0, -1.0,
                0, 0, 1, 0.0,
                0, 0, 0, 1.0;

        rot << cos(a), -sin(a), 0, 0,
               sin(a),  cos(a), 0, 0,
                    0,       0, 1, 0,
                    0,       0, 0, 1;

        scale << 2.0,     0,     0,   0,
                   0,   2.0,     0,   0,
                   0,     0,   2.0,   0,
                   0,     0,     0,   1.0;

        P << 1.0, 0.0, 0.0, 1.0;
                
        Matrix<double> final = shift * rot * scale;
        auto res = final * P;

        EXPECT_DOUBLE_EQ(3.914213562373095, res[0]);
        EXPECT_DOUBLE_EQ(0.4142135623730952, res[1]); 
        EXPECT_DOUBLE_EQ(0.0, res[2]);
        EXPECT_DOUBLE_EQ(1.0, res[3]);
    END
    
    return 0;
}
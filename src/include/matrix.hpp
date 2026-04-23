#ifndef MATRIX_H
#define MATRIX_H

#include <exception>
#include <cstddef>   
#include <iterator>
#include <iostream>

#include "matrix_error.hpp"

namespace genMatrix {
        enum __MatrixState {
            FixedStack,     /** A mátrix fix méretű, adatai a stacken vannak. (ez a default konstruktor érték is) */
            FixedHeap,      /** A mátrix fix méretű, de túl nagy a stackhez.  */
            DynamicHeap     /** A mátrix dinamikus méretű, adatai a heapen vannak. */
        };

        template<typename T> class Matrix {
        T staticBuf[5 * 5];             /** Kis (max. 5x5), fix méretű mátrixoknak.  */
        T* data;                        /** A mátrix elemei. */
        size_t n, m;                    /** A mátrix méretei (n sor, m oszlop). */
        __MatrixState dataLocation;     /** Eltárolja a mátrix adatainak helyét (heap vagy stack pointeren vannak az adatok). */

        /**
         * Megcseréli a és b-t.
         */
        void swap(T* a, T* b) {
            T tmp = *a;
            *a = *b;
            *b = tmp;
        }

        /**
         * Megállapítja, hogy a két mátrix pontosan egy méretű (nem csak az (n*m) szorzatuk az).
         */
        bool exact_size(const Matrix<T>& matA, const Matrix<T> matB) {
            return (matA.n == matB.n) && (matA.m && matB.m);
        }
    
    public:
        /* Segédosztályok deklarációi */

        class CommaInit;
        class Matrix_Iterator;

        /**
         * Létrehoz egy n*m-es mátrixot. Ha mind a két érték 0, akkor a mátrix dinamikus.
         * @param _n Sorok száma
         * @param _m Oszlopok száma
         */
        Matrix(size_t _n = 0, size_t _m = 0) : data(nullptr), n(_n), m(_m), dataLocation(FixedStack) {
            if (!n && !m) {
                dataLocation = DynamicHeap;
                return;
            }

            if (n <= 5 && m <= 5)  {
                data = staticBuf;
                return;
            }

            try {
                data = new T[this->size()](); /** forceoljuk a default constructort elemi típusokra is */
                dataLocation = FixedHeap;
            }
            catch (const std::bad_alloc&) {
                throw Matrix_Error("[Constructor]", "Memory allocation failure.", true);
            }
        }

        Matrix(const Matrix& other) : data(nullptr), n(0), m(0) {
            *this = other;
        }

        /** template-re nem működik saját típuson, át kell irányítani */
        Matrix& operator=(const Matrix& other) {
            if (this != &other) {
                this->operator=<T>(other);
            }

            return *this;
        }

        /**
         * Generikus másololó konstruktor
         */
        template<typename S> Matrix(const Matrix<S>& other) : data(nullptr), n(0), m(0) {
            *this = other;
        }

        /**
         * Generikus értékadó operátor
         */
        template<typename S> Matrix& operator=(const Matrix<S>& other) {
            if (dataLocation != FixedStack) delete[] data;

            dataLocation = other.getDataLocation();
            n = other.getRows();
            m = other.getCols();

            if (dataLocation == FixedStack) 
                data = staticBuf;
            else 
                data = new T[this->size()]();

            for (size_t i = 0; i < this->getRows(); i++) {
                for (size_t j = 0; j < this->getCols(); j++) {
                    this->operator()(i,j) = static_cast<T>(other(i,j));
                }
            }

            return *this;
        }

        /** 
         * Eigen stílusú mátrix feltöltés.
         * @param val A mátrix első értéke.
         * @note Dinamikus mátrix esetében ez nem használható.
         */
        CommaInit operator<<(const T& val) {
            if (dataLocation == DynamicHeap) 
                throw Matrix_Error("[operator<<]", "This assignment can only be used on known-size matrices.");

            return CommaInit(*this, val);
        }

        /**
         * @return A sorok száma.
         */
        size_t getRows() const { return n; }

        /**
         * @return Az oszlopok száma.
         */
        size_t getCols() const { return m; }

        /**
         * @return A mátrix mérete.
         */
        size_t size() const {
            if (!n) return m;
            if (!m) return n;
            return n * m;
        }

        __MatrixState getDataLocation() const { return dataLocation; }

        /**
         * @param row Sorindex
         * @param col Oszlopindex
         * @return A mátrix egy adott indexén lévő eleme.
         * @note A visszaadott elem nem módosítható.
         */
        const T& operator()(const size_t row, const size_t col) const {
            if (!n && !m) 
                throw Matrix_Error("[operator()]", "The matrix is empty.");

            if (row < 0 || row >= n || col < 0 || col >= m) 
                throw Matrix_Error("[operator()]", "Out of index");

            return data[row * m + col];
        }

        /**
         * @param row Sorindex
         * @param col Oszlopindex
         * @return A mátrix egy adott indexén lévő eleme.
         * @throws Matrix_Error kivétel túlindexelés esetén.
         */
        T& operator()(const size_t row, const size_t col) {
            if (!n && !m) 
                throw Matrix_Error("[operator()]", "The matrix is empty.");

            if (row < 0 || row >= n || col < 0 || col >= m) 
                throw Matrix_Error("[operator()]", "Out of index");

            return data[row * m + col];
        }

        /**
         * @return Igaz, ha mátrix minden eleme megegyezik (és ugyanakkorák). 
         */
        bool operator==(const Matrix<T>& other) {
            if (this == &other) return true;
            if (n != other.n || m != other.m || !(dataLocation == other.dataLocation)) return false;
            if (n == other.n == m == other.m == 0) return true;

            for (size_t i = 0; i < this->size(); i++)
                if (!type_numeric_eq<T>(data[i], other.data[i])) return false;

            return true;
        }

        Matrix_Iterator begin();
        Matrix_Iterator end();

        /**
         * Felcseréli a mátrix két sorát.
         * @param n1 Az egyik sor.
         * @param n2 A másik sor.
         * @throws Matrix_Error kivétel túlindexelés esetén.
         */
        Matrix& swapRow(size_t n1, size_t n2) {
            for (size_t i = 0; i < m; i++) {
                swap(&this->operator()(n1, i), &this->operator()(n2, i));
            }
            return *this;
        }

        /**
         * Felcseréli a mátrix két oszlopát.
         * @param n1 Az egyik oszlop.
         * @param n2 A másik oszlop.
         * @throws Matrix_Error kivétel túlindexelés esetén.
         */
        Matrix& swapCol(size_t m1, size_t m2) {
            for (size_t i = 0; i < n; i++) {
                swap(&this->operator()(i, m1), &this->operator()(i, m2));
            }
            return *this;
        }

        /**
         * Hozzáadja a mátrixot a kapott paraméterhez.
         * @param rhs_mtx A másik tag.
         * @return Az új mátrix az eredménnyel.
         * @throws Matrix_Error kivétel, ha az összeadás nem értelmezett.
         */
        Matrix operator+(const Matrix<T>& rhs_mtx) const {
            Matrix<T> rval = *this;
            rval += rhs_mtx;
            return rval;
        }

        /**
         * Hozzáadja a kapott paramétert a mátrixhoz.
         * @param rhs_type A másik tag.
         * @return Az új mátrix az eredménnyel.
         */
        Matrix operator+(const T& rhs_type) const {
            Matrix<T> rval = *this;
            rval += rhs_type;
            return rval;
        };

        /**
         * Hozzáadja a mátrixot a kapott paraméterhez.
         * @param rhs_mtx A másik tag.
         * @return A balérték referenciája.
         * @throws Matrix_Error kivétel, ha az összeadás nem értelmezett.
         */
        Matrix& operator+=(const Matrix<T>& rhs_mtx) {
            if (!exact_size(*this, rhs_mtx)) 
                throw Matrix_Error("[operator+=]", "Two matrices must have the same size (n * m).");

            for (size_t i = 0; i < this->size(); i++)
                data[i] = data[i] + rhs_mtx.data[i];

            return *this;
        };

        /**
         * Hozzáadja a kapott paramétert a mátrixhoz.
         * @param rhs_type A másik tag.
         * @return A balérték referenciája.
         */
        Matrix& operator+=(const T& rhs_type) {
            for (size_t i = 0; i < this->size(); i++)
                data[i] = data[i] + rhs_type;

            return *this;
        };


        /**
         * Kivonja a mátrixból a kapott paramétert.
         * @param rhs_mtx A másik tag.
         * @return Egy új mátrix az eredménnyel.
         * @throws Matrix_Error kivétel, ha a kivonás nem értelmezett.
         */
        Matrix operator-(const Matrix<T>& rhs_mtx) const {
            return *this + rhs_mtx * -1;
        }

        /**
         * Kivonja a kapott paramétert a mátrixhoz.
         * @param rhs_type A másik tag.
         * @return A balérték referenciája.
         */
        Matrix& operator-(const T& rhs_type) {
            *this += rhs_type * -1;
            return *this;
        }

        /**
         * Kivonja a mátrixból a kapott paramétert.
         * @param rhs_mtx A másik tag.
         * @return A balérték referenciája.
         * @throws Matrix_Error kivétel, ha a kivonás nem értelmezett.
         */
        Matrix& operator-=(const Matrix<T>& rhs_mtx) {
            *this += rhs_mtx * -1;
            return *this;
        }

        /**
         * Kivonja a kapott paramétert a mátrixhoz.
         * @param rhs_type A másik tag.
         * @return A balérték referenciája.
         */
        Matrix& operator-=(const T& rhs_type) {
            *this += rhs_type * -1;
            return *this;
        }

        /**
         * Összeszorozza a mátrixszal a kapott paramétert.
         * @param rhs_mtx A másik tag.
         * @return Egy új mátrix az eredménnyel.
         * @throws Matrix_Error kivétel, ha a szorzás nem értelmezett.
         */
        Matrix operator*(const Matrix<T>& rhs_mtx) const {
            if (m != rhs_mtx.n) 
                throw Matrix_Error("[operator*]", "Columns of this and rows of other must be equal.");

            Matrix<T> rval(n, rhs_mtx.m);
            for (size_t i = 0; i < n; i++)
                for (size_t k = 0; k < m; k++)
                    for (size_t j = 0; j < rhs_mtx.m; j++)
                        rval(i, j) += this->operator()(i, k) * rhs_mtx(k, j);
                        
            return rval;
        }

        /**
         * Összeszorozza a kapott paramétert a mátrixszal.
         * @param rhs_type A másik tag.
         * @return Egy új mátrix az eredménnyel.
         */
        Matrix operator*(const T& rhs_type) const {
            Matrix<T> rval = *this;
            rval *= rhs_type;
            return rval;
        }

        /**
         * Összeszorozza a mátrixszal a kapott paramétert.
         * @param rhs_mtx A másik tag.
         * @return A balérték referenciája.
         * @throws Matrix_Error kivétel, ha a szorzás nem értelmezett.
         */
        Matrix& operator*=(const Matrix<T>& rhs_mtx) {
            Matrix<T> tmp = this * rhs_mtx;
            this = tmp;
            return *this; 
        }

        /**
         * Összeszorozza a kapott paramétert a mátrixszal.
         * @param rhs_type A másik tag.
         * @return A balérték referenciája.
         */
        Matrix& operator*=(const T& rhs_type) {
            for (size_t i = 0; i < this->size(); i++) {
                data[i] = data[i] * rhs_type;
            }
            return *this;
        }

        /**
         * @brief Transzponálja a mátrixot.
         * @details A transzponálás során a mátrix i,j-dik elemét felcseréljük a j,i-dik elemével. 
         *          A transzponált mátrix mérete m*n-es lesz. Szimmetrikus mátrix transzponáltja önmaga.
         */
        void transposeInPlace() {
            if (n == m) {
                for (size_t i = 0; i < n; i++)
                    for (size_t j = i + 1; j < m; j++)
                        swap(&this->operator()(i,j), &this->operator()(j, i));
            }
            else {
                Matrix<T> tmp(m, n);
                for (size_t i = 0; i < n; i++)
                    for (size_t j = 0; j < m; j++)
                        tmp(j, i) = this->operator()(i, j);
                *this = tmp;
            }
        }

        /**
         * @brief Transzponálja a mátrixot.
         * @details A transzponálás során a mátrix i,j-dik elemét felcseréljük a j,i-dik elemével. 
         *          A transzponált mátrix mérete m*n-es lesz. Szimmetrikus mátrix transzponáltja önmaga.
         */
        Matrix transpose() {
            Matrix<T> tmp = *this;
            tmp.transposeInPlace();
            return tmp;
        };

        ~Matrix() {
            if (dataLocation != FixedStack)
                delete[] data;
            data = nullptr; /* double delete ellen */
            n = m = 0;
        };
    };
}

#endif
/**
 * @file genMatrix.hpp
 * @author Kovács Botond
 * @brief Generikus mátrix osztály sablonja.
 * @note A C++ sablonokat inline kell megvalósítani. Mivel ez a kód jelentős része, az egyszerűség kedvéért az a pár kiszervezhető
 *       függvény is inline.
 */

#ifndef GENMATRIX
#define GENMATRIX

#include <exception> /* std::exeption örökléséhez */
#include <cstddef>   
#include <iterator>
#include <iostream>

#include "type_func.hpp"


namespace genMatrix {
    template<typename T> class Matrix {
        T* data;        /** A mátrix elemei. */
        size_t n, m;    /** A mátrix méretei (n sor, m oszlop). */
        bool isDynamic; /** Eltárolja, hogy a mátrix dinamikus-e. */

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
        Matrix(size_t _n = 0, size_t _m = 0) : data(nullptr), n(_n), m(_m), isDynamic(false) {
            if (!n && !m) {
                isDynamic = true;
                return;
            }

            try {
                data = new T[this->size()](); /** forceoljuk a default constructort elemi típusokra is */
            }
            catch (const std::bad_alloc&) {
                throw "hibaosztaly ide";
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
            delete[] data;

            isDynamic = other.getDynamic();
            n = other.getRows();
            m = other.getCols();

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
            if (isDynamic) throw "csak statikus mereture hivhato";
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

        bool getDynamic() const { return isDynamic; }

        /**
         * @param row Sorindex
         * @param col Oszlopindex
         * @return A mátrix egy adott indexén lévő eleme.
         * @note A visszaadott elem nem módosítható.
         */
        const T& operator()(const size_t row, const size_t col) const {
            if (!n && !m) throw "ures mtx";
            if (row < 0 || row >= n || col < 0 || col >= m) throw "tulindexeles";
            return data[row * m + col];
        }

        /**
         * @param row Sorindex
         * @param col Oszlopindex
         * @return A mátrix egy adott indexén lévő eleme.
         * @throws Matrix_Error kivétel túlindexelés esetén.
         */
        T& operator()(const size_t row, const size_t col) {
            if (!n && !m) throw "ures mtx";
            if (row < 0 || row >= n || col < 0 || col >= m) throw "tulindexeles";
            return data[row * m + col];
        }

        /**
         * @return Igaz, ha mátrix minden eleme megegyezik (és ugyanakkorák). 
         */
        bool operator==(const Matrix<T>& other) {
            if (this == &other) return true;
            if (n != other.n || m != other.m || !(isDynamic == other.isDynamic)) return false;
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
            if (!exact_size(*this, rhs_mtx)) throw "nem egy meret";
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
            if (m != rhs_mtx.n) throw "nem jok a meretek";
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

        /**
         * @return A mátrix inverze.
         * @throws Matrix_Error kivétel, ha nem létezik.
         */
        Matrix inverse();

        ~Matrix() {
            delete[] data;
            data = nullptr; /* double delete ellen */
            n = m = 0;
        };
    };

    /**
    * @struct CommaInit
    * @brief Segédosztály az Eigen stílusú mátrixfeltöltéshez.
    * @details Egy segédosztály segítségével, a mátrixot fel lehet tölteni sorfolytonosan ((0,0) ... (0,m) ... (1, 0) ...).
    *          Ez időt spórol ha fel kell tölteni egy fix méretű mátrixot. Értékadás során pontosan annyi értéket kell megadni, 
    *          amekkora a mátrix mérete, ezért csak ismert, fix méret esetén alkalmazható.
    * @note Ez a segédosztály az egyszerűsége miatt teljesen public.
    */
    template<typename T> class Matrix<T>::CommaInit {
    public:
        Matrix<T>& mtx;  /** A cél mátrix referenciája. */
        size_t nextidx;  /** A következő üres index mtx.data-ban. */

        /**
        * Berakja a mátrixba az első elemmet.
        * @param _mtx A feltöltendő mátrix referenciája.
        * @param init Az első érték.
        */
        CommaInit(Matrix<T>& _mtx, const T& init) : mtx(_mtx), nextidx(0) {
            mtx.data[nextidx++] = init;
        }

        /**
        * Az mtx referencián elérhető mátrixot tölti fel a következő elemmel.
        * @param val A következő üres helyre szánt érték.
        * @return Önmaga referenciája, így lehet láncban hívni.
        */
        CommaInit& operator,(const T& rhs_val) {
            if (nextidx == mtx.size()) throw "tul sok parameter";
            mtx.data[nextidx++] = rhs_val;
            return *this;
        }

        ~CommaInit() noexcept(false) {
            if (nextidx != mtx.size()) throw "tul keves parameter";
        }
    };

    /**
     * @class Matrix_Iterator
     * @brief Iterátor osztály a Matrix<T>-hez.
     * @details Ez az iterátor implementálja a C++ szabvány szerinti operátorokat, amiket a std::random_access_iterator
     *          megkövetel. A függvények nincsenek kommentelve, mert igazából pointer wrapperek.
     * @note Operátor hadsereg osztály.
     */
    template<typename T> class Matrix<T>::Matrix_Iterator {
        T* ptr;
        
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        /**
         * @note A konstruktor azért explicit, hogy megakadályozzuk az implicit típuskonverzióból származó bugokat. Például
         *       raw pointer adunk egy olyan helyre ahol iteratort várunk.
         */
        explicit Matrix_Iterator(T* _ptr = nullptr);

        reference operator*() const;
        pointer operator->() const;
        reference operator[](difference_type rhs_diff) const;

        Matrix_Iterator& operator++();
        Matrix_Iterator operator++(int);
        Matrix_Iterator& operator--();
        Matrix_Iterator operator--(int);

        Matrix_Iterator  operator+(difference_type rhs_diff) const;
        Matrix_Iterator  operator-(difference_type rhs_diff) const;
        Matrix_Iterator& operator+=(difference_type rhs_diff);
        Matrix_Iterator& operator-=(difference_type rhs_diff);

        difference_type operator-(const Matrix_Iterator& rhs_iter) const;

        /* Sajnos a C++ szabvány megköveteli ezt az operátort (ehhez az iterátorhoz), ezt meg csak friendként (vagy global) lehet implementálni. */
        friend Matrix_Iterator operator+(difference_type lhs_diff, const Matrix_Iterator& rhs_iter);
        
        bool operator==(const Matrix_Iterator& rhs_iter) const;
        bool operator!=(const Matrix_Iterator& rhs_iter) const;
        bool operator<=(const Matrix_Iterator& rhs_iter) const;
        bool operator>=(const Matrix_Iterator& rhs_iter) const;
        bool operator>(const Matrix_Iterator& rhs_iter) const;
        bool operator<(const Matrix_Iterator& rhs_iter) const;

    };

    /**
     * @class Matrix_Error
     * @brief A Matrix osztály hibaosztálya.
     * @note Mivel a Matrix osztály egy template, a fordítási idő csökkentése érdekében nem az "eredeti" osztály része.
     */
    class Matrix_Error : public std::exception {
        bool isFatal;   /** Kritikus hiba-e? */
        char *src_func; /** Függvény neve, ahol a hiba történt */
        char *msg;      /** Hibaüzenet */

    public:
        /**
         * @param _src A függvény neve, ahol a hiba keletkezett.
         * @param _msg A hibaüzenet
         * @param fatal Kritikus-e a hiba? (ki kell-e lépni), alapértelmezetten hamis
         */
        Matrix_Error(const char *_src, const char *_msg, bool fatal = false);

        Matrix_Error(const Matrix_Error& other);

        /**
         * @return A függvény neve, ahol a hiba történt (0-val lezárt C string).
         */
        const char *src() const noexcept;

        /**
         * @return A hibaüzenet (0-val lezárt C string).
         */
        const char* what() const noexcept override;

        ~Matrix_Error();
    };
}

#endif
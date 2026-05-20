/**
 * @file matrix.hpp
 * @author Kovács Botond
 * @brief Generikus mátrix osztály deklarációja.
 */
#ifndef MATRIX_H
#define MATRIX_H

#include <cstddef>   
#include <iterator>

#include "matrix_error.hpp"
#include "random_iterator.hpp"


namespace genMatrix {

/** Vektor deklaráció */
template<typename T> class Vector;

enum __MatrixState {
    FixedStack,     /** A mátrix fix méretű, adatai a stacken vannak. */
    FixedHeap,      /** A mátrix fix méretű, de túl nagy a stackhez.  */
    DynamicHeap     /** A mátrix dinamikus méretű, adatai a heapen vannak. */
};

template<typename T> 
class Matrix {
    T staticBuf[5 * 5];             /** SBO kis mátroxoknak.  */
    T* data;                        /** A mátrix elemeinek mutatója. */
    size_t n;                       /** A mátrix sorainak száma */
    size_t m;                       /** A mátrix oszlopainak száma. */
    __MatrixState dataLocation;     /** Eltárolja, hogy hova mutat data. */

    /**
     * @brief Megcseréli a és b-t.
     */
    void swap(T* a, T* b) {
        T tmp = *a;
        *a = *b;
        *b = tmp;
    }

    /**
     * @brief Megállapítja, hogy a két mátrix pontosan egy méretű nem csak a size().
     */
    bool exact_size(const Matrix<T>& matA, const Matrix<T> matB) {
        return (matA.n == matB.n) && (matA.m && matB.m);
    }

public:
    /* Segédosztályok deklarációi */
    class CommaInit;

    using iter = internal::types::RandomIterator<T, T*, T&>;
    using const_iter = internal::types::RandomIterator<const T, const T*, const T&>;

    /**
     * @brief Létrehoz egy n*m-es mátrixot. Ha mind a két érték 0, akkor a mátrix dinamikus.
     * @param _n Sorok száma
     * @param _m Oszlopok száma
     */
    Matrix(size_t _n = 0, size_t _m = 0);

    /**
     * @brief Dummy copy konstruktor. Át van vezetve értékadásra.
     */
    Matrix(const Matrix& other);

    /**
     * @brief Dummy assignment operator. Át van vezetve generikus értékadásra.
     */
    Matrix& operator=(const Matrix& other);

    /**
     * @brief Dummy generikus másoló konstruktor. Át van vezetve generikus értékadásra.
     * @warning A paraméter mátrix típusának (S) castolhatónak kell lennie T-re.
     */
    template<typename S> 
    Matrix(const Matrix<S>& other);

    /**
     * @brief Generikus értékadó operátor.
     * @warning A paraméter mátrix típusának (S) castolhatónak kell lennie T-re.
     */
    template<typename S> 
    Matrix& operator=(const Matrix<S>& other);

    /**
     * @brief Move konstruktor: a jobbértéket a balba teszi copy nélkül.
     * @note Ha other adatai a stacken vannak, sajnos át kell copyzni azt.
     */
    Matrix(Matrix&& other);

    /**
     * @brief Move operátor: a jobbértéket a balba teszi copy nélkül.
     * @note Ha other adatai a stacken vannak, sajnos át kell copyzni azt.
     */
    Matrix& operator=(Matrix&& other);

    /** 
     * @brief Eigen stílusú mátrix feltöltés.
     * @param val A mátrix első értéke.
     * @note Dinamikus mátrix esetében ez nem használható.
     */
    template<typename S>
    CommaInit operator<<(const S& val);

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
        if (!n) 
            return m;
        if (!m) 
            return n;
        return n * m;
    }

    /**
     * @return Visszaadja hova mutat a data mutató. 
     *         (A pontos értékekért ld. __MatrixState).
     */
    __MatrixState getDataLocation() const { return dataLocation; }

    /**
     * @param row Sorindex
     * @param col Oszlopindex
     * @return A mátrix egy adott indexén lévő eleme.
     * @note A visszaadott elem nem módosítható.
     */
    const T& operator()(const size_t row, const size_t col) const;

    /**
     * @param row Sorindex
     * @param col Oszlopindex
     * @return A mátrix egy adott indexén lévő eleme.
     * @throws Matrix_Error kivétel túlindexelés esetén.
     */
    T& operator()(const size_t row, const size_t col);

    /**
     * @return Igaz, ha mátrix minden eleme megegyezik (és ugyanakkorák).
     * @note Eltérő típus esetén hamis. 
     */
    template<typename S>
    bool operator==(const Matrix<S>& other) const;

    /**
     * @return RandomIterator, ami mátrix adatainak elejére mutat.
     */
    iter begin() noexcept { return iter(data); }

    /**
     * @return RandomIterator, ami mátrix adatainak vége UTÁN mutat.
     */
    iter end() noexcept { return iter(data + this->size()); }

    /**
     * @return Konstans RandomIterator, ami mátrix adatainak elejére mutat.
     */
    const_iter cbegin() const noexcept { return const_iter(data); }

    /**
     * @return Konstans RandomIterator, ami mátrix adatainak vége UTÁN mutat.
     */
    const_iter cend() const noexcept { return const_iter(data + this->size()); }

    /**
     * Felcseréli a mátrix két sorát.
     * @param n1 Az egyik sor.
     * @param n2 A másik sor.
     * @throws Matrix_Error kivétel túlindexelés esetén.
     */
    Matrix& swapRow(size_t n1, size_t n2);

    /**
     * Felcseréli a mátrix két oszlopát.
     * @param n1 Az egyik oszlop.
     * @param n2 A másik oszlop.
     * @throws Matrix_Error kivétel túlindexelés esetén.
     */
    Matrix& swapCol(size_t m1, size_t m2);

    /**
     * @brief Újraméretezi a dinamikus mátrixot.
     * @param _n Az új sorméret
     * @param _m Az új oszlopméret
     * @throws Matrix_Error kivétel, ha a mátrix nem dinamikus.
     * @warning Ha az új méret kisebb, mint a meglévő, adatvesztés történhet.
     * @note Ha valamelyik paraméter 0, a mátrix törli a tartalmát.
     */
    void resize(size_t _n, size_t _m);

    /**
     * Hozzáadja a mátrixot a kapott paraméterhez.
     * @param rhs_mtx A másik tag.
     * @return Az új mátrix az eredménnyel.
     * @throws Matrix_Error kivétel, ha az összeadás nem értelmezett.
     */
    template<typename S>
    decltype(auto) operator+(const Matrix<S>& rhs_mtx) const;

    /**
     * Hozzáadja a kapott paramétert a mátrixhoz.
     * @param rhs_type A másik tag.
     * @return Az új mátrix az eredménnyel.
     */
    template<typename S>
    decltype(auto) operator+(const S& rhs_type) const;

    /**
     * Hozzáadja a mátrixot a kapott paraméterhez.
     * @param rhs_mtx A másik tag.
     * @return A balérték referenciája.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem egyeznek.
     */
    Matrix& operator+=(const Matrix<T>& rhs_mtx);

    /**
     * Hozzáadja a kapott paramétert a mátrixhoz.
     * @param rhs_type A másik tag.
     * @return A balérték referenciája.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem egyeznek.
     */
    template<typename S>
    Matrix& operator+=(const S& rhs_type);

    /**
     * Kivonja a mátrixból a kapott paramétert.
     * @param rhs_mtx A másik tag.
     * @return Egy új mátrix az eredménnyel.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem egyeznek.
     */
    template<typename S>
    decltype(auto) operator-(const Matrix<S>& rhs_mtx) const;

    /**
     * Kivonja a kapott paramétert a mátrixhoz.
     * @param rhs_type A másik tag.
     * @return A balérték referenciája.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem egyeznek.
     */
    template<typename S>
    decltype(auto) operator-(const S& rhs_type) const;

    /**
     * Kivonja a mátrixból a kapott paramétert.
     * @param rhs_mtx A másik tag.
     * @return A balérték referenciája.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem egyeznek.
     */
    Matrix& operator-=(const Matrix<T>& rhs_mtx);

    /**
     * Kivonja a kapott paramétert a mátrixhoz.
     * @param rhs_type A másik tag.
     * @return A balérték referenciája.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem egyeznek.
     */
    template<typename S>
    Matrix& operator-=(const S& rhs_type);

    /**
     * Összeszorozza a mátrixszal a kapott paramétert.
     * @param rhs_mtx A másik tag.
     * @return Egy új mátrix az eredménnyel.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem megfelelőek.
     */
    template<typename S>
    decltype(auto) operator*(const Matrix<S>& rhs_mtx) const;

    /**
     * Összeszorozza a mátrixszal a kapott paramétert.
     * @param rhs_vec A másik tag.
     * @return Egy új vektor az eredménnyel.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem megfelelőek.
     */
    template<typename S>
    decltype(auto) operator*(const Vector<S>& rhs_vec) const;

    /**
     * Összeszorozza a kapott paramétert a mátrixszal.
     * @param rhs_type A másik tag.
     * @return Egy új mátrix az eredménnyel.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem megfelelőek.
     */
    template<typename S>
    decltype(auto) operator*(const S& rhs_type) const;

    /**
     * Összeszorozza a mátrixszal a kapott paramétert.
     * @param rhs_mtx A másik tag.
     * @return A balérték referenciája.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem megfelelőek.
     */
    Matrix& operator*=(const Matrix<T>& rhs_mtx);

    /**
     * Összeszorozza a mátrixszal a kapott paramétert.
     * @param rhs_vec A másik tag.
     * @return A balérték referenciája.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem megfelelőek.
     */
    Matrix& operator*=(const Vector<T>& rhs_vec);

    /**
     * Összeszorozza a kapott paramétert a mátrixszal.
     * @param rhs_type A másik tag.
     * @return A balérték referenciája.
     * @throws Matrix_Error, ha a műveletre nincs definiált operátor.
     * @throws Matrix_Error, ha a méretek nem megfelelőek.
     */
    template<typename S>
    Matrix& operator*=(const S& rhs_type);

    /**
     * @brief Transzponálja a mátrixot helyben.
     * @details A transzponálás során a mátrix i,j-dik elemét felcseréljük a 
     *          j,i-dik elemével. A transzponált mátrix mérete m*n-es lesz. 
     *          Szimmetrikus mátrix transzponáltja önmaga.
     */
    void transposeInPlace();

    /**
     * @brief Transzponálja a mátrixot.
     * @details A transzponálás során a mátrix i,j-dik elemét felcseréljük a 
     *          j,i-dik elemével. A transzponált mátrix mérete m*n-es lesz. 
     *          Szimmetrikus mátrix transzponáltja önmaga.
     */
    Matrix transpose() const;

    /**
     * @brief Kiszámítja a mátrix inverzét. 
     * @details Inverz mátrix kiszámítása a Gauss-Jordan eliminációval. Az inverz 
                mátrix kiszámításához eliminálni kell az (M|E) mátrixot, ahol E az
                egységmátrix és M az eredeti. Az elmináció végén M redukált lépcsős
                alakba kerül és E lesz az inverz mátrix. A determinánshoz hasonlóan
                a pontosságot a mátrix elemei és kondíciója is befolyásolják.
     * @return Az inverz mátrix.
     * @warning Az inverz mátrix nem feltétlenül azonos típusú az eredetivel.
     */
    decltype(auto) inverse() const;

    ~Matrix();
};
}

#include "matrix.ipp"

#endif
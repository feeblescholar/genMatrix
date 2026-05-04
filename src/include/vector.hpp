/**
 * @file vector.hpp
 * @author Kovács Botond
 * @brief Generikus vektor deklaráció.
 */
#ifndef VECTOR_H
#define VECTOR_H

#include "matrix.hpp"
#include "vector_error.hpp"

namespace genMatrix {

template<typename T>
class Vector {
    T* data;
    size_t _size;
    size_t _capacity;

public:
    /**
     * @brief Létrehoz egy n magas oszlopvektort.
     * @param n A vektor mérete.
     */
    Vector(size_t n = 1);

    /**
     * @brief Létrehoz egy oszlopvektort egy megadott mátrix oszlopából.
     * @param mtx A forrásmátrix referenciája.
     * @param idx A forrásmátrix oszlopának indexe.
     */
    Vector(const Matrix<T>& mtx, const size_t idx);

    /**
     * @note Át van vezetve értékadásra.
     */
    Vector(const Vector& other);

    /**
     * @brief Generikus másoló konstruktor. Át van vezetve értékadásra.
     */
    template<typename S> 
    Vector(const Vector<S>& other);

    /**
     * @brief Move konstruktor: átrakja a jobbértéket a balba copy nélkül.
     */
    Vector(Vector&& other);

    /**
     * @brief Dummy értékadó operátor. Át van vezetve generikus értékadásra.
     */
    Vector& operator=(const Vector& other);

    /**
     * @brief Generikus értékadó operátor.
     * @warning T-nek castolhatónak kell lennie S-re
     */
    template<typename S>
    Vector& operator=(const Vector<S>& other);

    /**
     * @brief Move operátor: átrakja a jobbértéket a balba copy nélkül.
     * @returns A balérték referenciája.
     */
    Vector& operator=(Vector&& other);

    /**
     * @return Az elemek száma.
     */
    size_t size() { return _size; };

    /**
     * @return A vektor maximális kapacitása.
     */
    size_t capacity() { return _capacity; }

    /**
     * @brief Konstans indexelő operátor.
     * @return Konstans referencia az indexen lévő adattagra.
     * @throw const char* kivétel túlindexelés esetén
     */
    const T& operator[](const size_t idx) const;

    /**
     * @brief Indexelő operátor.
     * @return Referencia az indexen lévő adattagra.
     * @throw const char* kivétel túlindexelés esetén
     */
    T& operator[](const size_t idx);

    /**
     * @brief Hozzáad egy elemet a vektorhoz.
     * @param val Az új érték referenciája.
     * @return Önmaga referenciája, így lehet láncban hívni.
     */
    Vector& push_back(const T& val);

    /**
     * @brief Újraméretezi a vektort a paraméternek megfelelően.
     * @param ncap Az új kapacitás érték.
     */
    void resize(const size_t ncap);

    /**
     * @brief Lecsökkenti a vektor kapacitását a méretére.
     */
    void shrink();

    /**
     * @brief Kiszámítja a vektor (Euklideszi) normáját.
     * @return A vektor hossza.
     */
    T length() const {
        T sum = 0;

        for (size_t i = 0; i < _size; i++)
            sum = std::hypot(sum, data[i]);
        
        return sum;
    }

    /**
     * @brief Generikus komparátor. Igaz, ha két vektor mérete, típusa és 
     *        tartalma igaz.
     */
    template<typename S>
    bool operator==(const Vector<S>& rhs_vec) const;

    /**
     * @brief Hozzáadja a kapott paramétert a Vektorhoz.
     * @param rhs_vec A másik tag referenciája.
     * @return Egy új vektor az eredménnyel.
     * @throws Vector_Error kivétel, ha a két vektor nem azonos méretű.
     * @warning Type promotion lehetséges, ha T != S.
     */
    template<typename S>
    decltype(auto) operator+(const Vector<S>& rhs_vec) const;

    /**
     * @brief Hozzáadja a kapott paramétert a balértékhez (in-place).
     * @return A balérték referenciája.
     * @throws Vector_Error kivétel, ha a két vektor nem azonos méretű.
     */
    Vector& operator+=(const Vector& rhs_vec);

    /**
     * @brief Kivonja a kapott paramétert a Vektorból.
     * @param rhs_vec A másik tag referenciája.
     * @return Egy új vektor az eredménnyel.
     * @throws Vector_Error kivétel, ha a két vektor nem azonos méretű.
     * @warning Type promotion lehetséges, ha T != S.
     */
    template<typename S>
    decltype(auto) operator-(const Vector<S>& rhs_vec) const;

    /**
     * @brief Kivonja a kapott paramétert a balértékből (in-place).
     * @throws Vector_Error kivétel, ha a két vektor nem azonos méretű.
     * @return A balérték referenciája.
     */
    Vector& operator-=(const Vector& rhs_vec);

    /**
     * @brief Skalárisan összeszorozza this-t a kapott paraméterrel.
     * @throws Vector_Error kivétel, ha a két vektor nem azonos méretű.
     * @return Skalár, melynek a típusa, this és a paraméter szorzatából képzett.
     */
    template<typename S>
    decltype(auto) operator*(const Vector<S>& rhs_vec) const;

    /**
     * @brief Megszorozza a vektort egy konstanssal.
     * @return Egy új vektor az eredménnyel.
     * @warning Type promotion lehetséges, ha T != S.
     */
    template<typename S>
    decltype(auto) operator*(const S& rhs_type) const;

    ~Vector();

};
}

#include "vector_constructors.ipp"
#include "vector_assignment.ipp"
#include "vector_accessors.ipp"
#include "vector_sizemanip.ipp"
#include "vector_binaryop.ipp"

#endif

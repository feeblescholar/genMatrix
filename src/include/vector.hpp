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

    ~Vector();

};
}

#include "vector_constructors.ipp"
#include "vector_assignment.ipp"
#include "vector_accessors.ipp"
#include "vector_sizemanip.ipp"

#endif

/**
 * @file complex.hpp
 * @author Kovács Botond
 * @brief genMatrix kompatibilis komplex osztály definicója.
 */
#ifndef COMPLEX_H
#define COMPLEX_H

#include <cmath>

namespace genMatrix {
    
class Complex {
    double re; /** Valós rész. */
    double im; /** Képzetes rész. */

    /** Megtiljuk az implicit double konverziót. */
    operator double() const { return 0.0; }

public:
    /**
     * @param _re A komplex szám valós része.
     * @param _im A komplex szám képzetes része.
     * @note Az alapértelmezett érték: (0 + 0i).
     * @note Az implicit konverzió az osztálysablonok saját operátorai miatt le
     *       van tiltva, ugyanis a compiler nem tudja, hogy mi legyen.
     */
    explicit Complex(double _re, double _im) : re(_re), im(_im) {}

    explicit Complex(double _re) :  re(_re), im(0) {}

    explicit Complex() : re(0), im(0) {}

    /**
     * @return A komplex szám valós része.
     */
    double getRe() const { return re; }

    /**
     * @return A komplex szám képzetes része.
     */
    double getIm() const { return im; };

    /**
     * @return A komplex szám abszolútértéke.
     */
    double abs() const { return sqrt(re*re + im*im); }

    /**
     * @brief Beállítja a valós részt.
     * @param _re A valós rész új értéke
     */
    void setRe(double _re) { re = _re; }

    /**
     * @brief Beállítja a képzetes részt.
     * @param _im A képzetes rész új értéke
     */
    void setIm(double _im) { im = _im; }

    /**
     * @brief Komplex komparátor.
     * @return Igaz, ha a valós és képzetes részek is megegyeznek.
     */
    bool operator==(const Complex& rhs_c) const;

    /**
     * @brief Nem egyenlőség vizsgálat.
     * @return Igaz, ha a valós és/vagy a képezetes részek eltérnek.
     */
    bool operator!=(const Complex& rhs_c) const;

    /**
     * @return Egy új komplex szám, ami konjugált.
     */
    Complex operator~() const;

    /**
     * @brief Összeadja a komplex számot a kapott komplex paraméterrel.
     * @param rhs_c A másik tag referenciája.
     * @return Egy új komplex szám az eredménnyel.
     */
    Complex operator+(const Complex& rhs_c) const;

    /**
     * @brief Összeadja a komplex számot a paraméterként kapott elemi típussal.
     * @param rhs_type Az elemi típus referenciája.
     * @return Egy új komplex szám az eredménnyel.
     * @note A paraméter a valós részhez adódik hozzá.
     */
    template<typename T>
    Complex operator+(const T& rhs_type) const;

    /**
     * @brief Hozzáadja a kapott komplex paramétert a balértékhez.
     * @param rhs_c A másik tag referenciája.
     * @return A balérték referenciája.
     */
    Complex& operator+=(const Complex& rhs_c);

    /**
     * @brief Hozzáadja a kapott elemi típust a balértékhez.
     * @param rhs_type Az elemi típus referenciája.
     * @return A balérték referenciája
     * @note A paraméter a valós részhez adódik hozzá.
     */
    template<typename T>
    Complex& operator+=(const T& rhs_type);

    /**
     * @brief Kivonja a komplex számból a kapott komplex paramétert.
     * @param rhs_c A másik tag referenciája.
     * @return Egy új komplex szám az eredménnyel.
     */
    Complex operator-(const Complex& rhs_c) const;

    /**
     * @brief Kivonja a komplex számból a paraméterként kapott elemi típust.
     * @param rhs_type Az elemi típus referenciája.
     * @return Egy új komplex szám az eredménnyel.
     * @note A paraméter a valós részhez adódik hozzá.
     */
    template<typename T>
    Complex operator-(const T& rhs_type) const;

    /**
     * @brief Kivnja a balértékből a kapott komplex paramétert.
     * @param rhs_c A másik tag referenciája.
     * @return A balérték referenciája.
     */
    Complex& operator-=(const Complex& rhs_c);

    /**
     * @brief Kivonja a balértékből a kapott elemi típust.
     * @param rhs_type Az elemi típus referenciája.
     * @return A balérték referenciája
     * @note A paraméter a valós részhez adódik hozzá.
     */
    template<typename T>
    Complex& operator-=(const T& rhs_type);

    /**
     * @brief Kiszámítja a komplex szám és a kapott paraméter szorzatát.
     * @param rhs_c A másik tényező referenciája.
     * @return Egy új komplex szám az eredménnyel.
     */
    Complex operator*(const Complex& rhs_c) const;

    /**
     * @brief Kiszámítja a komplex szám és a paraméterként kapott elemi típus
     *        szorzatát.
     * @param rhs_type Az elemi típus referenciája.
     * @return Egy új komplex szám az eredménnyel.
     */
    template<typename T>
    Complex operator*(const T& rhs_type) const;

    /**
     * @brief Kiszámítja a komplex szám és a paraméter szorzatát a balértékbe.
     * @param rhs_c A másik tényező referenciája.
     * @return A balérték referenciája.
     */
    Complex& operator*=(const Complex& rhs_c);

    /**
     * @brief Megszorozza a balértéket a paraméterként kapott elemi típussal.
     * @param rhs_type Az elemi típus referenciája.
     * @return A balérték referenciája.
     */
    template<typename T>
    Complex& operator*=(const T& rhs_type);

    /**
     * @brief Leosztja a komplex számot a kapott paraméterrel.
     * @param rhs_c Az osztó referenciája.
     * @return Egy új komplex szám az eredménnyel.
     */
    Complex operator/(const Complex& rhs_c) const;

    /**
     * @brief Leosztja a balértéket a kapott paraméterrel.
     * @param rhs_c Az osztó referenciája.
     * @return Egy új komplex szám az eredménnyel.
     */
    Complex& operator/=(const Complex& rhs_c);
};

template<typename T>
Complex operator+(const T& lhs_type, const Complex& rhs_c);

template<typename T>
Complex operator*(const T& lhs_type, const Complex& rhs_c);

/**
 * @brief Kiírja a paraméterként kapott komplex számot a megadott output streamre.
 * @param os A cél output stream referenciája.
 * @param rhs_c A kiírandó komplex szám referenciája.
 * @return Az output stream referenciája.
 */
std::ostream& operator<<(std::ostream& os, const Complex& rhs_c);

/**
 * @brief Beolvas egy komplex számot a megadott input streamről.
 * @param is A forrás input stream referenciája.
 * @param rhs_c A cél komplex szám referenciája.
 * @return Az input stream referenciája.
 */
std::istream& operator>>(std::istream& is, Complex& rhs_c);
}

#endif

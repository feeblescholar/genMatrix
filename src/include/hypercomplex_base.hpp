/**
 * @file hypercomplex_base.hpp
 * @author Kovács Botond
 * @brief Hiperkomplex számok bázisosztályainak deklarációja.
 * @note A bővíthetőség céljából.
 */
#ifndef HYPERCOMPLEX_BASE_H
#define HYPERCOMPLEX_BASE_H

namespace genMatrix::internal::types {

/**
 * @class Hypercomplex
 * @brief A hiperkomplex számok bázisosztálya.
 * @note Ezúton lehet saját type traitje minden gyereknek.
 */
class Hypercomplex {
    public:
        virtual ~Hypercomplex() {}
};

/**
 * @class Hypercomplex2D
 * @brief 2 dimenziós hiperkomplex számok absztrakt bázisosztálya.
 */
class Hypercomplex2D : public Hypercomplex {
protected:
    double re; /** Valós rész */
    double im; /** Képzetes rész */

public:
    /**
     * @param _re A hiperkomplex valós része.
     * @param _im A hiperkomplex képzetes része.
     * @note Az alapértelmezett érték (0, 0).
     */
    Hypercomplex2D(double _re = 0, double _im = 0) : re(_re), im(_im) {}

    /**
     * @return A szám valós része.
     */
    double real() const { return re; }

    /**
     * @return A szám "képzetes" része.
     */
    double unreal() const { return im; };

    /**
     * @return A szám normája.
     */
    virtual double abs() const = 0;

    /**
     * @return A "képzetes egység" négyzete.
     */
    virtual double unit_sq() const = 0;
};
}

#endif
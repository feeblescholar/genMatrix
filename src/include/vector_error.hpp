/**
 * @file matrix_error.hpp
 * @author Kovács Botond
 * @brief A vektor hibaosztályának deklarációja.
 */
#ifndef VECTOR_ERROR_H
#define VECTOR_ERROR_H

#include <cstring>

#include "error_base.hpp"

namespace genMatrix {
/**
 * @class Matrix_Error
 * @brief A vektor osztály hibaosztálya.
 * @note A clean hierarchia érdekében nem a Matrix_Error hibaosztály gyereke.
 */
class Vector_Error : public genMatrix_Error {
    bool isFatal;   /** Kritikus hiba-e? */
    char *src_func; /** Függvény neve, ahol a hiba történt */
    char *msg;      /** Hibaüzenet */

public:
    /**
     * @param _src A függvény neve, ahol a hiba keletkezett.
     * @param _msg A hibaüzenet
     * @param fatal Kritikus-e a hiba? (ki kell-e lépni), alapból hamis
     */
    Vector_Error(const char *_src, const char *_msg, bool fatal = false);

    /**
     * @note A C++ szabvány megköveteli a copy konstruktort.
     */
    Vector_Error(const Vector_Error& other);

    /**
     * @return A függvény neve, ahol a hiba történt (0-val lezárt C string).
     */
    const char *src() const noexcept override;

    /**
     * @return A hibaüzenet (0-val lezárt C string).
     */
    const char* what() const noexcept override;

    ~Vector_Error();
};
}

#endif

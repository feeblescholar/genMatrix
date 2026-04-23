#ifndef MATRIX_ERROR_H
#define MATRIX_ERROR_H

#include <exception>
#include <cstring>

namespace genMatrix {
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
/**
 * @file error_base.hpp
 * @author Kovács Botond
 * @brief A genMatrix hibaosztályainak absztrakt bázisosztálya.
 */
#ifndef ERROR_BASE_H
#define ERROR_BASE_H

#include <exception>

namespace genMatrix::internal::types {
/**
 * @class genMatrix_Error
 * @brief A genMatrix hibaosztályainak absztrakt bázisosztálya.
 */
class genMatrix_Error : public std::exception {
public:
    /**
     * @return A forrásfüggvény üzenet mutatója.
     */
    virtual const char* src() const noexcept = 0;

    /**
     * @return A hibaüzenet.
     */
    virtual const char* what() const noexcept = 0;

    virtual ~genMatrix_Error() {}
};
}

#endif

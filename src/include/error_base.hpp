/**
 * @file error_base.hpp
 * @author Kovács Botond
 * @brief A genMatrix hibaosztályainak absztrakt bázisosztálya.
 */
#ifndef ERROR_BASE_H
#define ERROR_BASE_H

#include <exception>

class genMatrix_Error : public std::exception {
public:
    virtual const char* src() const noexcept = 0;

    virtual const char* what() const noexcept = 0;

    virtual ~genMatrix_Error() {}
};

#endif

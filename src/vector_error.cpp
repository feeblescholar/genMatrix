/**
 * @file vector_error.cpp
 * @author Kovács Botond
 * @brief A vektor hibaosztályának definiciói.
 */
#include "include/vector_error.hpp"

using namespace genMatrix;

Vector_Error::Vector_Error(const char *_src, const char *_msg, bool fatal) : 
    isFatal(fatal), 
    src_func(nullptr), 
    msg(nullptr) {
        
    src_func = new char[std::strlen(_src) + 1];
    msg = new char[std::strlen(_msg) + 1];

    std::strcpy(msg, _msg);
    std::strcpy(src_func, _src);
}

Vector_Error::Vector_Error(const Vector_Error& other) 
    : isFatal(other.isFatal), 
      src_func(nullptr), 
      msg(nullptr) {
    
    src_func = new char[std::strlen(other.src_func) + 1];
    msg = new char[std::strlen(other.msg) + 1];

    std::strcpy(msg, other.msg);
    std::strcpy(src_func, other.src_func);
}


const char* Vector_Error::src() const noexcept {
    return src_func;
}

const char* Vector_Error::what() const noexcept {
    return msg;
}

Vector_Error::~Vector_Error() {
    delete[] src_func;
    delete[] msg;

    msg = src_func = nullptr;
}

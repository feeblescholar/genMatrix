#include "matrix_error.hpp"

using namespace genMatrix;

Matrix_Error::Matrix_Error(const char *_src, const char *_msg, bool fatal) : isFatal(fatal), src_func(nullptr), msg(nullptr) {
    src_func = new char[std::strlen(_src) + 1];
    msg = new char[std::strlen(_msg) + 1];

    std::strcpy(msg, _msg);
    std::strcpy(src_func, _src);
}

Matrix_Error::Matrix_Error(const Matrix_Error& other) : isFatal(false), src_func(nullptr), msg(nullptr) {
    src_func = new char[std::strlen(other.src_func) + 1];
    msg = new char[std::strlen(other.msg) + 1];
    isFatal = other.isFatal;

    std::strcpy(msg, other.msg);
    std::strcpy(src_func, other.src_func);
}


const char* Matrix_Error::src() const noexcept {
    return src_func;
}

const char* Matrix_Error::what() const noexcept {
    return msg;
}

Matrix_Error::~Matrix_Error() {
    delete[] src_func;
    delete[] msg;

    msg = src_func = nullptr;
}
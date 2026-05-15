/**
 * @file complex.ipp
 * @author Kovács Botond
 * @brief genMatrix kompatibilis komplex osztály implementáció.
 */
#ifndef COMPLEX_I
#define COMPLEX_I

#include "utils.hpp"

namespace genMatrix {

template<typename T>
Complex& Complex::operator+=(const T& rhs_type) {
	re += rhs_type;
	return *this;
}

template<typename T>
Complex Complex::operator-(const T& rhs_type) const {
	return Complex(re - rhs_type, im);
}

template<typename T>
Complex& Complex::operator-=(const T& rhs_type) {
	re -= rhs_type;
	return *this;
}

template<typename T>
Complex Complex::operator*(const T& rhs_type) const {
	return Complex(rhs_type * re, rhs_type * im);
}

template<typename T>
Complex& Complex::operator*=(const T& rhs_type) {
	re *= rhs_type;
	im *= rhs_type;

	return *this;
}

template<typename T>
Complex Complex::operator/(const T& rhs_type) const {
	if (genMatrix::utils::eq(rhs_type, T(0)))
		throw std::domain_error("Division by zero.");

	return Complex(re / static_cast<double>(rhs_type), im / static_cast<double>(rhs_type));
}

template<typename T>
Complex& Complex::operator/=(const T& rhs_type) {
    *this = *this / rhs_type;
    return *this;
}

template<typename T>
typename std::enable_if_t<std::is_arithmetic_v<T>, Complex>
operator+(const T& lhs_type, const Complex& rhs_c) {
	return Complex(rhs_c).operator+=(lhs_type);
}

template<typename T>
typename std::enable_if_t<std::is_arithmetic_v<T>, Complex>
operator-(const T& lhs_type, const Complex& rhs_c) {
    return Complex(lhs_type - rhs_c.getRe(), rhs_c.getIm());
}

template<typename T>
typename std::enable_if_t<std::is_arithmetic_v<T>, Complex>
operator*(const T& lhs_type, const Complex& rhs_c) {
	return Complex(rhs_c).operator*=(lhs_type);
}
}

#endif

/**
 * @file complex.ipp
 * @author Kovács Botond
 * @brief genMatrix kompatibilis komplex osztály implementáció.
 */
#ifndef COMPLEX_I
#define COMPLEX_I

#include <iostream>
#include <iomanip>

#include "utils.hpp"

namespace genMatrix {

bool Complex::operator==(const Complex& rhs_c) const {
	return utils::eq(re, rhs_c.re) && utils::eq(im, rhs_c.im);
}

bool Complex::operator!=(const Complex& rhs_c) const {
    return !(*this == rhs_c);
}

Complex Complex::operator~() const {
	return Complex(re,(-1) * im);
}

Complex Complex::operator+(const Complex& rhs_c) const {
	return Complex(re + rhs_c.re, im + rhs_c.im);
}

template<typename T>
Complex Complex::operator+(const T& rhs_type) const {
	return Complex(re + rhs_type, im);
}

Complex& Complex::operator+=(const Complex& rhs_c) {
	re += rhs_c.re;
	im += rhs_c.im;

	return *this;
}

template<typename T>
Complex& Complex::operator+=(const T& rhs_type) {
	re += rhs_type;
	return *this;
}

Complex Complex::operator-(const Complex& rhs_c) const {
	return Complex(re - rhs_c.re, im - rhs_c.im);
}

template<typename T>
Complex Complex::operator-(const T& rhs_type) const {
	return Complex(re - rhs_type, im);
}

Complex& Complex::operator-=(const Complex& rhs_c) {
	re -= rhs_c.re;
	im -= rhs_c.im;

	return *this;
}

template<typename T>
Complex& Complex::operator-=(const T& rhs_type) {
	re -= rhs_type;
	return *this;
}

Complex Complex::operator*(const Complex& rhs_c) const {
	double nRe = re * rhs_c.re - im * rhs_c.im;
	double nIm = re * rhs_c.im + im * rhs_c.re;

	return Complex(nRe, nIm);
}

template<typename T>
Complex Complex::operator*(const T& rhs_type) const {
	return Complex(rhs_type * re, rhs_type * im);
}


Complex& Complex::operator*=(const Complex& rhs_c) {
	double tmp = re * rhs_c.re - im * rhs_c.im;

	im = re * rhs_c.im + im * rhs_c.re;
	re = tmp;

	return *this;
}

template<typename T>
Complex& Complex::operator*=(const T& rhs_type) {
	re *= rhs_type;
	im *= rhs_type;

	return *this;
}

Complex Complex::operator/(const Complex& rhs_c) const {
	if (utils::eq(rhs_c.re, 0.0) && utils::eq(rhs_c.im, 0.0))
		throw std::domain_error("Division by zero.");

	double div = rhs_c.re * rhs_c.re + rhs_c.im * rhs_c.im;
	double nRe = (re * rhs_c.re + im * rhs_c.im) / div;
	double nIm = (im * rhs_c.re - re * rhs_c.im) / div;

	return Complex(nRe, nIm);
}

template<typename T>
Complex Complex::operator/(const T& rhs_type) const {
	if (genMatrix::utils::eq(rhs_type, T(0)))
		throw std::domain_error("Division by zero.");

	return Complex(re / static_cast<double>(rhs_type), im / static_cast<double>(rhs_type));
}

Complex& Complex::operator/=(const Complex& rhs_c) {
	*this = *this / rhs_c;
	return *this;
}

template<typename T>
Complex& Complex::operator/=(const T& rhs_type) {
    *this = *this / rhs_type;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Complex& rhs_c) {
	os << rhs_c.getRe();

	if (rhs_c.getIm() >= 0) 
		os << std::showpos;
	os << rhs_c.getIm() << "j";

	os << std::noshowpos;
	return os;
}

std::istream& operator>>(std::istream& is, Complex& rhs_c) {
	double re = 0;
	double im = 0;
	char i;

	is >> re >> std::ws;
	is >> im >> std::ws;

	rhs_c.setRe(re);
	rhs_c.setIm(im);

	is >> i;
	return is;
}
}

#endif

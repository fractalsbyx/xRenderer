#ifndef COMPLEX_H
#define COMPLEX_H

#include "typedefs.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

struct Complex {
  coordType x;
  coordType y;
  Complex() {}
  Complex(coordType x_val, coordType y_val) : x(x_val), y(y_val) {}
  Complex(const Complex &other) : x(other.x), y(other.y) {}

  // Binary operators of complex numbers
  Complex operator+(const Complex &other) const {
    return Complex(x + other.x, y + other.y);
  }
  Complex operator-(const Complex &other) const {
    return Complex(x - other.x, y - other.y);
  }
  Complex operator*(const Complex &other) const {
    return Complex(x * other.x - y * other.y, x * other.y + y * other.x);
  }
  Complex operator/(const Complex &other) const {
    coordType denom = other.x * other.x + other.y * other.y;
    return Complex((x * other.x + y * other.y) / denom,
                   (y * other.x - x * other.y) / denom);
  }

  // Binary operators of complex numbers with scalar
  Complex operator+(const coordType &scalar) const {
    return Complex(x + scalar, y);
  }
  Complex operator-(const coordType &scalar) const {
    return Complex(x - scalar, y);
  }
  Complex operator*(const coordType &scalar) const {
    return Complex(x * scalar, y * scalar);
  }
  Complex operator/(const coordType &scalar) const {
    return Complex(x / scalar, y / scalar);
  }

  // Assignment operators with complex numbers
  Complex &operator=(const Complex &other) {
    if (this != &other) {
      x = other.x;
      y = other.y;
    }
    return *this;
  }
  Complex &operator+=(const Complex &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Complex &operator-=(const Complex &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  Complex &operator*=(const Complex &other) {
    coordType temp_x = x;
    x                = x * other.x - y * other.y;
    y                = temp_x * other.y + y * other.x;
    return *this;
  }
  Complex &operator/=(const Complex &other) {
    coordType denom  = other.x * other.x + other.y * other.y;
    coordType temp_x = x;
    x                = (x * other.x + y * other.y) / denom;
    y                = (y * other.x - temp_x * other.y) / denom;
    return *this;
  }

  // Assignment operators with scalar
  Complex &operator=(const coordType &scalar) {
    x = scalar;
    y = 0;
    return *this;
  }
  Complex &operator+=(const coordType &scalar) {
    x += scalar;
    return *this;
  }
  Complex &operator-=(const coordType &scalar) {
    x -= scalar;
    return *this;
  }
  Complex &operator*=(const coordType &scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  Complex &operator/=(const coordType &scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  // Unary operators
  Complex operator-() const { return Complex(-x, -y); }
  Complex operator+() const { return *this; }
  Complex operator~() const { return Complex(x, -y); }

  // Other functions

  Complex   real() const { return Complex(x, 0); }
  Complex   imag() const { return Complex(0, y); }
  Complex   conjugate() const { return Complex(x, -y); }
  coordType mag() const { return std::sqrt(x * x + y * y); }
  coordType mag2() const { return x * x + y * y; }

  coordType arg() const { return std::atan2(y, x); }
  Complex   exp() const {
    return Complex(std::exp(x) * std::cos(y), std::exp(x) * std::sin(y));
  }
  Complex log() const { return Complex(std::log(mag()), arg()); }

  Complex sqrt() const {
    return Complex(std::sqrt(mag()) * std::cos(arg() / 2),
                   std::sqrt(mag()) * std::sin(arg() / 2));
  }
  Complex sin() const {
    return Complex(std::sin(x) * std::cosh(y), std::cos(x) * std::sinh(y));
  }
  Complex cos() const {
    return Complex(std::cos(x) * std::cosh(y), -std::sin(x) * std::sinh(y));
  }
  Complex tan() const { return sin() / cos(); }
  Complex sinh() const {
    return Complex(std::sinh(x) * std::cos(y), std::cosh(x) * std::sin(y));
  }
  Complex cosh() const {
    return Complex(std::cosh(x) * std::cos(y), std::sinh(x) * std::sin(y));
  }
  Complex tanh() const { return sinh() / cosh(); }
  Complex asin() const {
    return Complex(std::asin(x) * std::cosh(y), std::atan2(std::sinh(y), x));
  }
  Complex acos() const {
    return Complex(std::acos(x) * std::cosh(y), std::atan2(std::sinh(y), x));
  }
  Complex atan() const {
    return Complex(std::atan(x) * std::cosh(y), std::atan2(std::sinh(y), x));
  }
  Complex asinh() const {
    return Complex(std::asinh(x) * std::cos(y), std::atan2(std::sinh(y), x));
  }
  Complex acosh() const {
    return Complex(std::acosh(x) * std::cos(y), std::atan2(std::sinh(y), x));
  }
  Complex atanh() const {
    return Complex(std::atanh(x) * std::cos(y), std::atan2(std::sinh(y), x));
  }
  Complex abs() const { return Complex(std::abs(x), std::abs(y)); }

  // Power functions
  Complex pow(const Complex &other) const {
    return Complex(std::pow(mag(), other.x) * std::cos(other.y * arg()),
                   std::pow(mag(), other.x) * std::sin(other.y * arg()));
  }
  Complex pow(const coordType &other) const {
    return Complex(std::pow(mag(), other) * std::cos(other * arg()),
                   std::pow(mag(), other) * std::sin(other * arg()));
  }
  // Coordinate conversion functions
  Complex polar(const coordType &r, const coordType &theta) const {
    return Complex(r * std::cos(theta), r * std::sin(theta));
  }
  Complex cartesian(const coordType &r, const coordType &theta) const {
    return Complex(r * std::cos(theta), r * std::sin(theta));
  }

  // Comparison operators
  // bool operator==(const Complex &other) const {
  //   return (x == other.x && y == other.y);
  // }
  // bool operator!=(const Complex &other) const { return !(*this == other); }
};

inline std::ostream &operator<<(std::ostream &os, const Complex &c) {
  os << "(" << c.x << ", " << c.y << ")";
  return os;
}

inline std::istream &operator>>(std::istream &is, Complex &c) {
  char ch;
  is >> ch;  // Read the opening parenthesis
  is >> c.x; // Read the x value
  is >> ch;  // Read the comma
  is >> c.y; // Read the y value
  is >> ch;  // Read the closing parenthesis
  return is;
}
inline Complex operator+(const coordType &scalar, const Complex &c) {
  return Complex(scalar + c.x, c.y);
}
inline Complex operator-(const coordType &scalar, const Complex &c) {
  return Complex(scalar - c.x, -c.y);
}
inline Complex operator*(const coordType &scalar, const Complex &c) {
  return Complex(scalar * c.x, scalar * c.y);
}
inline Complex operator/(const coordType &scalar, const Complex &c) {
  coordType denom = c.x * c.x + c.y * c.y;
  return Complex((scalar * c.x) / denom, (-scalar * c.y) / denom);
}
inline Complex   real(const Complex &c) { return c.real(); }
inline Complex   imag(const Complex &c) { return c.imag(); }
inline Complex   conj(const Complex &c) { return c.conjugate(); }
inline coordType mag(const Complex &c) { return c.mag(); }
inline coordType mag2(const Complex &c) { return c.mag2(); }
inline coordType arg(const Complex &c) { return c.arg(); }
inline Complex   exp(const Complex &c) { return c.exp(); }
inline Complex   log(const Complex &c) { return c.log(); }
inline Complex   sqrt(const Complex &c) { return c.sqrt(); }
inline Complex   sin(const Complex &c) { return c.sin(); }
inline Complex   cos(const Complex &c) { return c.cos(); }
inline Complex   tan(const Complex &c) { return c.tan(); }
inline Complex   sinh(const Complex &c) { return c.sinh(); }
inline Complex   cosh(const Complex &c) { return c.cosh(); }
inline Complex   tanh(const Complex &c) { return c.tanh(); }
inline Complex   asin(const Complex &c) { return c.asin(); }
inline Complex   acos(const Complex &c) { return c.acos(); }
inline Complex   atan(const Complex &c) { return c.atan(); }
inline Complex   asinh(const Complex &c) { return c.asinh(); }
inline Complex   acosh(const Complex &c) { return c.acosh(); }
inline Complex   atanh(const Complex &c) { return c.atanh(); }
inline Complex   abs(const Complex &c) { return c.abs(); }

#endif // COMPLEX_H
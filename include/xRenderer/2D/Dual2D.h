#ifndef DUAL2D_H
#define DUAL2D_H

#include "Vector2D.h"
#include <xRenderer/2D/Vector2D.h>
#include <xRenderer/core/typedefs.h>

struct Dual2D {
  // Dual2D() : a(0), b(0, 0) {}
  Dual2D(realType _a = 0, Vector2D _b = Vector2D(0, 0)) : a(_a), b(_b) {}

  Dual2D
  operator+(const Dual2D &other) const {
    return Dual2D(a + other.a, b + other.b);
  }
  Dual2D
  operator-(const Dual2D &other) const {
    return Dual2D(a - other.a, b - other.b);
  }
  Dual2D
  operator*(const Dual2D &other) const {
    return Dual2D(a * other.a, a * other.b + b * other.a);
  }
  Dual2D
  operator/(const Dual2D &other) const {
    // if (other.a == 0) { throw std::invalid_argument("Division by zero"); }
    return Dual2D(a / other.a,
                  (a * other.b - b * other.a) / (other.a * other.a));
  }

  Dual2D
  operator+(const realType &constant) const {
    return Dual2D(a + constant, b);
  }
  Dual2D
  operator-(const realType &constant) const {
    return Dual2D(a - constant, b);
  }
  Dual2D
  operator*(const realType &constant) const {
    return Dual2D(a * constant, b * constant);
  }
  Dual2D
  operator/(const realType &constant) const {
    // if (constant == 0) { throw std::invalid_argument("Division by zero"); }
    return Dual2D(a / constant, b / constant);
  }

  Dual2D
  operator-() const {
    return Dual2D(-a, -b);
  }

  template <typename T>
  Dual2D
  operator+=(const T &other) {
    *this = *this + other;
    return *this;
  }
  template <typename T>
  Dual2D
  operator-=(const T &other) {
    *this = *this - other;
    return *this;
  }
  template <typename T>
  Dual2D
  operator*=(const T &other) {
    *this = *this * other;
    return *this;
  }
  template <typename T>
  Dual2D
  operator/=(const T &other) {
    *this = *this / other;
    return *this;
  }

  realType a;
  Vector2D b;
};

inline Dual2D
operator*(const realType constant, const Dual2D &d) {
  return Dual2D(d.a * constant, d.b * constant);
}
inline Dual2D
operator/(const realType constant, const Dual2D &d) {
  // if (d.a == 0) { throw std::invalid_argument("Division by zero"); }
  return Dual2D(constant) / d;
}

inline Dual2D
sin(const Dual2D &d) {
  return Dual2D(std::sin(d.a), std::cos(d.a) * d.b);
}

inline Dual2D
cos(const Dual2D &d) {
  return Dual2D(std::cos(d.a), -std::sin(d.a) * d.b);
}

inline Dual2D
sqrt(const Dual2D &d) {
  return Dual2D(std::sqrt(d.a), d.b / (2 * std::sqrt(d.a)));
}

inline Dual2D
pow(const Dual2D &d, const realType &exponent) {
  return Dual2D(std::pow(d.a, exponent),
                d.b * exponent * std::pow(d.a, exponent - 1));
}

#endif // DUAL2D_H
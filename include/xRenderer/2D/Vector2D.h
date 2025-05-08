#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <xRenderer/core/typedefs.h>

struct Vector2D {
  Vector2D() : x(0), y(0) {}
  Vector2D(realType x, realType y) : x(x), y(y) {}

  Vector2D
  operator+(const Vector2D &other) const {
    return Vector2D(x + other.x, y + other.y);
  }

  Vector2D
  operator-(const Vector2D &other) const {
    return Vector2D(x - other.x, y - other.y);
  }

  Vector2D
  operator*(const realType &scalar) const {
    return Vector2D(x * scalar, y * scalar);
  }

  Vector2D
  operator/(const realType &scalar) const {
    // if (scalar == 0) { throw std::invalid_argument("Division by zero"); }
    return Vector2D(x / scalar, y / scalar);
  }

  Vector2D
  operator+=(const Vector2D &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Vector2D
  operator-=(const Vector2D &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Vector2D
  operator*=(const realType &scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  Vector2D
  operator/=(const realType &scalar) {
    // if (scalar == 0) { throw std::invalid_argument("Division by zero"); }
    x /= scalar;
    y /= scalar;
    return *this;
  }

  Vector2D
  operator-() const {
    return Vector2D(-x, -y);
  }

  realType
  mag2() const {
    return x * x + y * y;
  }

  realType
  mag() const {
    return std::sqrt(mag2());
  }

  realType x;
  realType y;
};

inline Vector2D
operator*(const realType scalar, const Vector2D &v) {
  return Vector2D(v.x * scalar, v.y * scalar);
}

realType
dot(const Vector2D &v1, const Vector2D &v2) {
  return v1.x * v2.x + v1.y * v2.y;
}
realType
cross(const Vector2D &v1, const Vector2D &v2) {
  return v1.x * v2.y - v1.y * v2.x;
}
realType
distance(const Vector2D &v1, const Vector2D &v2) {
  return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) +
                   (v1.y - v2.y) * (v1.y - v2.y));
}
realType
mag2(const Vector2D &v) {
  return v.mag2();
}
realType
mag(const Vector2D &v) {
  return v.mag();
}

#endif // VECTOR2D_H
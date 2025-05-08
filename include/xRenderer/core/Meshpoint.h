#ifndef MESHPOINT_H
#define MESHPOINT_H

#include <xRenderer/core/Complex.h>
#include <xRenderer/core/typedefs.h>

struct Meshpoint {
  meshInt x_index;
  meshInt y_index;

  Complex toComplex() const {
    return Complex(realType(x_index), realType(y_index));
  }

  Meshpoint(meshInt x, meshInt y) : x_index(x), y_index(y) {}
};

#endif // MESHPOINT_H
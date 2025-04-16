#ifndef MESHPOINT_H
#define MESHPOINT_H

#include "Complex.h"
#include "typedefs.h"

struct Meshpoint {
  meshInt x_index;
  meshInt y_index;

  Complex toComplex() const {
    return Complex(coordType(x_index), coordType(y_index));
  }

  Meshpoint(meshInt x, meshInt y) : x_index(x), y_index(y) {}
};

#endif // MESHPOINT_H
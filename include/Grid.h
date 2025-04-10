#ifndef GRID2D_H
#define GRID2D_H

#include "Meshpoint.h"
#include "typedefs.h"
#include <stdexcept>

template <typename dataType> class Grid2D {
public:
  Grid2D() = default;
  Grid2D(meshInt x, meshInt y) : xres(x), yres(y), capacity(x * y) {
    if (capacity > 0) {
      data = new dataType[capacity];
    }
  }
  virtual ~Grid2D() { delete[] data; }

  dataType &operator()(meshInt x, meshInt y) {
    if (x >= xres || y >= yres) {
      throw std::out_of_range("Index out of range");
    }
    return data[y * xres + x];
  }

  dataType &operator()(const Meshpoint &mPoint) {
    if (mPoint.x_index >= xres || mPoint.y_index >= yres) {
      throw std::out_of_range("Index out of range");
    }
    return data[mPoint.y_index * xres + mPoint.x_index];
  }

  const dataType &operator()(meshInt x, meshInt y) const {
    if (x >= xres || y >= yres) {
      throw std::out_of_range("Index out of range");
    }
    return data[y * xres + x];
  }

  meshInt size() const { return capacity; }
  meshInt x_res() const { return xres; }
  meshInt y_res() const { return yres; }
  void recapacity(meshInt x, meshInt y) {
    delete[] data;
    xres = x;
    yres = y;
    capacity = x * y;
    if (capacity > 0) {
      data = new dataType[capacity];
    } else {
      data = nullptr;
    }
  }
  void clear() {
    delete[] data;
    data = nullptr;
    xres = 0;
    yres = 0;
    capacity = 0;
  }

private:
  meshInt xres = 0;
  meshInt yres = 0;
  meshInt capacity = xres * yres;
  dataType *data = nullptr;
};

#endif // Grid2D_H
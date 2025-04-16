#ifndef GRID2D_H
#define GRID2D_H

#include "Meshpoint.h"
#include "typedefs.h"
#include <stdexcept>

template <typename dataType> class Grid2D {
public:
  Grid2D(){};
  Grid2D(meshInt x, meshInt y) : xres(x), yres(y), capacity(x * y) {
    if (capacity > 0) { data = new dataType[capacity]; }
  }
  virtual ~Grid2D() { delete[] data; }

  dataType &operator()(const Meshpoint &mPoint) const {
    if (mPoint.x_index >= xres || mPoint.y_index >= yres) {
      throw std::out_of_range("Index out of range");
    }
    return data[mPoint.y_index * xres + mPoint.x_index];
  }

  dataType &operator()(const meshInt &x, const meshInt &y) const {
    if (x >= xres || y >= yres) {
      throw std::out_of_range("Index out of range");
    }
    return data[y * xres + x];
  }

  dataType &operator()(meshInt &linear_index) const {
    if (linear_index >= capacity) {
      throw std::out_of_range("Index out of range");
    }
    return data[linear_index];
  }

  Meshpoint getMeshPoint(const meshInt &linear_index) const {
    if (linear_index >= capacity) {
      throw std::out_of_range("Index out of range");
    }
    meshInt x = linear_index % xres;
    meshInt y = linear_index / xres;
    return Meshpoint(x, y);
  }

  meshInt size() const { return capacity; }
  meshInt x_res() const { return xres; }
  meshInt y_res() const { return yres; }

  void resize(meshInt x, meshInt y) {
    delete[] data;
    xres     = x;
    yres     = y;
    capacity = x * y;
    if (capacity > 0) {
      data = new dataType[capacity];
    } else {
      data = nullptr;
    }
  }

  void clear() {
    delete[] data;
    data     = nullptr;
    xres     = 0;
    yres     = 0;
    capacity = 0;
  }

  const dataType *getData() const { return data; }

private:
  meshInt   xres     = 0;
  meshInt   yres     = 0;
  meshInt   capacity = 0;
  dataType *data     = nullptr;
};

#endif // Grid2D_H
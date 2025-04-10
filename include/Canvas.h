#ifndef CANVAS_H
#define CANVAS_H

#include <Color.h>
#include <Grid2D.h>
#include <Meshpoint.h>
#include <XYMapping.h>
using coordType = double;
template <short unsigned int xres, short unsigned int yres> class Canvas {
public:
  Canvas(const Canvas &) = delete;
  Canvas &operator=(const Canvas &) = delete;

  void setLocation(short unsigned int x, short unsigned int y,
                   const coordType &value) {
    Complex(x, y) = value;
  }

private:
  Grid2D<coordType, xres, yres> Complex;
  Grid2D<Color, xres, yres> image;
};

#endif // CANVAS_H
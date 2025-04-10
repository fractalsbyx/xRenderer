#ifndef XYMAPPING_H
#define XYMAPPING_H

#include "Complex.h"
#include "Meshpoint.h"
#include "typedefs.h"
#include <cmath>
#include <unordered_map>
class XYMapping {
public:
  Complex getComplex(Meshpoint mPoint) const {
    auto it = cache.find(mPoint);
    if (it != cache.end()) {
      return it->second;
    }
    const Complex input_coord(coordType(mPoint.x_index),
                              coordType(mPoint.y_index));

    // const Complex returnVal =
    //     center + rot_vec * scale * (input_coord - (spatial_extent / 2.0)) /
    //                  (greater_dim / 2.0);

    const Complex returnVal =
        center + rot_vec * scale *
                     (2.0 * input_coord - (spatial_extent - 1.0)) /
                     (greater_dim - 1.0);

    cache[mPoint] = returnVal;
    return returnVal;
  }

  void setCenter(const Complex &c) {
    center = c;
    cache.clear();
  }
  void setScale(const coordType &s) {
    scale = s;
    cache.clear();
  }
  void setRotation(const coordType &r) {
    rotation = r;
    rot_vec.x = std::cos(rotation * (use_degrees ? 1.0 : (M_PI / 180.0)));
    rot_vec.y = std::sin(rotation * (use_degrees ? 1.0 : (M_PI / 180.0)));
    cache.clear();
  }
  void setLocation(const coordType &r, const Complex &c) {
    rotation = r;
    center = c;
    rot_vec.x = std::cos(rotation * (use_degrees ? 1.0 : (M_PI / 180.0)));
    rot_vec.y = std::sin(rotation * (use_degrees ? 1.0 : (M_PI / 180.0)));
    cache.clear();
  }

private:
  const Complex spatial_extent = Complex(coordType(xres), coordType(yres));
  coordType greater_dim = coordType(std::max(xres, yres));

  // Dimensions of the grid
  meshInt xres = 0;
  meshInt yres = 0;
  meshInt size = xres * yres;

  // Inputs
  Complex center;
  coordType scale;
  coordType rotation;
  bool use_degrees = false;

  // Precomputed values
  Complex rot_vec;

  mutable std::unordered_map<Meshpoint, Complex> cache;
};

#endif // XYMAPPING_H
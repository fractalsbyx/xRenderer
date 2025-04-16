#ifndef XYMAPPING_H
#define XYMAPPING_H

#include "Complex.h"
#include "Meshpoint.h"
#include "typedefs.h"
#include <cmath>
#include <unordered_map>

class XYMapping {
public:
  XYMapping(const meshInt &x = 0, const meshInt &y = 0,
            const Complex &c = Complex(0.0, 0.0), const coordType s = 1.0,
            coordType r = 0.0, bool _use_degrees = false)
      : xres(x), yres(y), center(c), scale(s), rotation(r),
        use_degrees(_use_degrees) {
    resize(x, y);
    rot_vec.x = std::cos(rotation * (use_degrees ? 1.0 : (M_PI / 180.0)));
    rot_vec.y = std::sin(rotation * (use_degrees ? 1.0 : (M_PI / 180.0)));
  }

  Complex getLocation(const Complex &p) const {
    // AAauto it = cache.find(p);
    // AAif (it != cache.end()) { return it->second; }
    //  const Complex returnVal =
    //      center + rot_vec * scale * (p - (spatial_extent / 2.0)) /
    //                   (greater_dim / 2.0);

    const Complex returnVal = center + rot_vec * scale *
                                           (2.0 * p - (spatial_extent - 1.0)) /
                                           (greater_dim - 1.0);

    // AAcache[p] = returnVal;
    return returnVal;
  }

  void setCenter(const Complex &c) {
    center = c;
    // AAcache.clear();
  }
  void setScale(const coordType &s) {
    scale = s;
    // AAcache.clear();
  }
  void setRotation(const coordType &r) {
    rotation  = r;
    rot_vec.x = std::cos(rotation * (use_degrees ? 1.0 : (M_PI / 180.0)));
    rot_vec.y = std::sin(rotation * (use_degrees ? 1.0 : (M_PI / 180.0)));
    // AAcache.clear();
  }
  void setLocation(const Complex &c, const coordType &s,
                   const coordType &r = 0.0) {
    center    = c;
    scale     = s;
    rotation  = r;
    rot_vec.x = std::cos(rotation * (use_degrees ? 1.0 : (M_PI / 180.0)));
    rot_vec.y = std::sin(rotation * (use_degrees ? 1.0 : (M_PI / 180.0)));
    // AAcache.clear();
  }

  void resize(meshInt x, meshInt y) {
    xres           = x;
    yres           = y;
    spatial_extent = Complex(coordType(xres), coordType(yres));
    greater_dim    = coordType(std::max(xres, yres));
    // AAcache.clear();
  }

private:
  // Dimensions of the grid
  meshInt   xres;
  meshInt   yres;
  Complex   spatial_extent;
  coordType greater_dim;

  // Inputs
  Complex   center      = Complex(0.0, 0.0);
  coordType scale       = 2.0;
  coordType rotation    = 0.0;
  bool      use_degrees = false;

  // Precomputed values
  Complex rot_vec;

  // Cache for computed locations
  // AAmutable std::unordered_map<Complex, Complex> cache;
};

#endif // XYMAPPING_H
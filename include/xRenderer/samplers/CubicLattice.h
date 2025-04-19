#ifndef CUBICLATTICE_H
#define CUBICLATTICE_H

#include <xRenderer/color/Color.h>
#include <xRenderer/color/Gradient.h>
#include <xRenderer/core/Complex.h>
#include <xRenderer/core/typedefs.h>
#include <xRenderer/core/util.h>
#include <xRenderer/samplers/Sampler.h>

#include <cmath>

template <typename Color> class CubicLattice : public Sampler<Color> {
public:
  CubicLattice() = default;
  CubicLattice(const unsigned int &depth, const coordType &distance,
               const coordType &distance_law, const Complex &shift,
               const int &sharpness, std::shared_ptr<Gradient<Color>> grad)

      : depth(depth), distance(distance), distance_law(distance_law),
        shift(shift), sharpness(sharpness), gradient(grad) {}
  ~CubicLattice() override = default;

  Color sample(const Complex &p) const override {
    // return Color::yellow();
    coordType x = 0.0;
    for (unsigned int level = 1; level < depth; ++level) {
      coordType i = static_cast<coordType>(level) + distance;
      coordType d = std::sqrt(i * i + i * p.real() * i * p.real() +
                              i * p.imag() * i * p.imag());
      x += std::pow(d, -distance_law) *
           power( //
               std::cos(M_PI * (i * p.real() - shift.real())) *
                   std::cos(M_PI * (i * p.imag() - shift.imag())),
               2 * sharpness);
    }
    return gradient->getColor(x);
  }
  unsigned int                     depth        = 100;
  coordType                        distance     = 0.0;
  coordType                        distance_law = 0.5;
  Complex                          shift        = Complex(0.0, 0.0);
  int                              sharpness    = 32;
  std::shared_ptr<Gradient<Color>> gradient     = nullptr;
};

#endif // CUBICLATTICE_H
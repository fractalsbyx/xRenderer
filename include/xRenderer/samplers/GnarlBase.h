#ifndef GNARLBASE_H
#define GNARLBASE_H

#include <memory>
#include <xRenderer/2D/Dual2D.h>
#include <xRenderer/2D/Vector2D.h>
#include <xRenderer/color/Gradient.h>
#include <xRenderer/core/typedefs.h>
#include <xRenderer/samplers/Sampler.h>

class GnarlGenerator {
public:
  virtual Dual2D
  generator(const Dual2D &x, const Dual2D &y) {
    return x + y;
  }

  Vector2D
  swirl(const realType &x, const realType &y) {
    Dual2D gen =
        generator(Dual2D(x, Vector2D(1.0, 0.0)), Dual2D(y, Vector2D(0.0, 1.0)));
    return Vector2D(-gen.b.y, gen.b.x);
  }
};

template <typename Color>
class GnarlBase : public Sampler<Color> {
public:
  GnarlBase(const double &_total_time, const double &_time_step,
            const std::shared_ptr<GnarlGenerator>   $_generator,
            const std::shared_ptr<Gradient<Color>> &_gradient)
      : total_time(_total_time), time_step(_time_step),
        iterations(total_time / time_step), generator($_generator),
        gradient(_gradient) {}

  Complex
  gnarl(const Complex &p) const {
    Vector2D position(p.real(), p.imag());
    for (unsigned int i = 0; i < iterations; ++i) {
      position =
          position + generator->swirl(position.x, position.y) * time_step;
    }
    return Complex(position.x, position.y);
  }

  virtual Color
  sample(const Complex &p) const override {
    Complex end_pos = gnarl(p);
    return gradient->getColor(mag2(end_pos - p));
  }

  double                           total_time;
  double                           time_step;
  unsigned int                     iterations;
  std::shared_ptr<GnarlGenerator>  generator;
  std::shared_ptr<Gradient<Color>> gradient;
};

#endif // GNARLBASE_H
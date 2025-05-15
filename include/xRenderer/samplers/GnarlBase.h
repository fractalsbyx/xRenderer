#ifndef GNARLBASE_H
#define GNARLBASE_H

#include <xRenderer/2D/Dual2D.h>
#include <xRenderer/2D/Vector2D.h>
#include <xRenderer/color/Gradient.h>
#include <xRenderer/core/Complex.h>
#include <xRenderer/core/typedefs.h>
#include <xRenderer/samplers/BasicRealSamplers.h>
#include <xRenderer/samplers/Sampler.h>

#include <memory>

class GnarlGenerator {
public:
  virtual Dual2D
  generator(const Dual2D &x, const Dual2D &y) const {
    return x + y;
  }

  Vector2D
  swirl(const realType &x, const realType &y,
        const Complex &twistor = Complex(0.0, 1.0)) const {
    Dual2D gen =
        generator(Dual2D(x, Vector2D(1.0, 0.0)), Dual2D(y, Vector2D(0.0, 1.0)));
    Complex c = twistor * Complex(gen.b.x, gen.b.y);
    return Vector2D(c.real(), c.imag());
  }
};

template <typename Color>
class GnarlBase : public Sampler<Color> {
public:
  GnarlBase(const double &_total_time, const double &_time_step,
            const realType &angle, const bool &_use_difference,
            const std::shared_ptr<GnarlGenerator>    &_generator,
            const std::shared_ptr<Gradient<Color>>   &_gradient,
            const std::shared_ptr<Sampler<realType>> &_color_sampler =
                std::make_shared<RealSampler>())
      : total_time(_total_time), time_step(_time_step),
        iterations(total_time / time_step), twistor(polar(1.0, angle)),
        use_difference(_use_difference), generator(_generator),
        gradient(_gradient), color_sampler(_color_sampler) {}

  void
  set_angle(const realType &angle) {
    twistor = polar(1.0, angle);
  }

  Complex
  gnarl(const Complex &p) const {
    Vector2D position(p.real(), p.imag());
    for (unsigned int i = 0; i < iterations; ++i) {
      position = position +
                 generator->swirl(position.x, position.y, twistor) * time_step;
    }
    return Complex(position.x, position.y);
  }

  virtual Color
  sample(const Complex &p) const override {
    Complex end_pos = gnarl(p);
    return gradient->getColor(
        color_sampler->sample(use_difference ? (end_pos - p) : end_pos));
  }

  double                             total_time;
  double                             time_step;
  unsigned int                       iterations;
  Complex                            twistor        = Complex(0.0, 1.0);
  bool                               use_difference = true;
  std::shared_ptr<GnarlGenerator>    generator;
  std::shared_ptr<Gradient<Color>>   gradient;
  std::shared_ptr<Sampler<realType>> color_sampler;
};

#endif // GNARLBASE_H
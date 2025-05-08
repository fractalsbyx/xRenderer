#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <xRenderer/color/Color.h>
#include <xRenderer/color/Gradient.h>
#include <xRenderer/samplers/Sampler.h>

template <typename Color> class Mandelbrot : public Sampler<Color> {
public:
  Mandelbrot() = default;
  Mandelbrot(const unsigned int &max_iter, const realType &bailout_val,
             std::shared_ptr<Gradient<Color>> grad)
      : max_iterations(max_iter), bailout_value(bailout_val), gradient(grad) {}
  ~Mandelbrot() override = default;

  Color sample(const Complex &p) const override {
    // Implement the Mandelbrot sampling logic here
    // return Color::yellow();
    Complex      z(0, 0);
    Complex      c         = p;
    unsigned int curr_iter = 0;
    while (mag2(z) < bailout_value && curr_iter < max_iterations) {
      z = z * z + c;
      curr_iter++;
    }
    if (curr_iter >= max_iterations) { return Color(40, 10, 120); }
    return gradient->getColor(20.f * float(curr_iter));
  }
  unsigned int                     max_iterations = 1000;
  realType                         bailout_value  = 4.0;
  std::shared_ptr<Gradient<Color>> gradient       = nullptr;
};

#endif // MANDELBROT_H
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "IMWrite/stb_image_write.h"
//
#include "Canvas.h"
#include "Color.h"
#include "ColorSpaces.h"
#include "Complex.h"
#include "Gradient.h"
#include "Mapping.h"
#include "Sampler.h"
#include "SuperSampler.h"
#include "typedefs.h"
#include <cstdint>
#include <memory.h>
#include <memory>

class Mandelbrot : public Sampler<RGBA> {
public:
  Mandelbrot() = default;
  Mandelbrot(const unsigned int &max_iter, const coordType &bailout_val,
             std::shared_ptr<Gradient<RGBA>> grad)
      : max_iterations(max_iter), bailout_value(bailout_val), gradient(grad) {}
  ~Mandelbrot() override = default;

  RGBA sample(const Complex &p) const override {
    // Implement the Mandelbrot sampling logic here
    Complex      z(0, 0);
    Complex      c         = p;
    unsigned int curr_iter = 0;
    while (mag2(z) < bailout_value && curr_iter < max_iterations) {
      z = z * z + c;
      curr_iter++;
    }
    if (curr_iter == max_iterations) { return RGBA::black(); }
    return gradient->getColor(float(curr_iter));
  }
  unsigned int                    max_iterations = 1000;
  coordType                       bailout_value  = 4.0;
  std::shared_ptr<Gradient<RGBA>> gradient       = nullptr;
};

int main() {
  // Create a Canvas object
  constexpr meshInt xres = 800;
  constexpr meshInt yres = 600;

  Canvas<RGBA> canvas(xres, yres);

  // Make a gradient
  std::shared_ptr<Gradient<RGBA>> gradient;
  gradient->addNode(RGBA::cyan(), 0.0);
  gradient->addNode(RGBA::magenta(), 120.0);
  gradient->addNode(RGBA::yellow(), 240.0);

  // Make a Mandelbrot sampler
  std::shared_ptr<Sampler<RGBA>> sampler =
      std::make_shared<Mandelbrot>(1000, 4.0, gradient);

  // Identity mapping
  std::shared_ptr<Mapping> identity;

  // Access the Complex and image Grid2Ds
  canvas.setLocation(Complex(0.0, 0.0), 0.0);
  canvas.addLayer(sampler, identity);

  canvas.draw();

  if (stbi_write_png(
          "test1.png", int(xres), int(yres), 4,
          reinterpret_cast<const uint8_t *>(canvas.getImage().getData()),
          int(xres * 4))) {
    std::cout << "Image saved!\n";
  } else {
    std::cerr << "Failed to save image.\n";
  }
  return 0;
}
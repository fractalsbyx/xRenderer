#include <iostream>
#include <sys/types.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <IMWrite/stb_image_write.h>
#include <json/json.hpp>
//
#include <Canvas.h>
#include <Color.h>
#include <ColorSpaces.h>
#include <Complex.h>
#include <Gradient.h>
#include <Mapping.h>
#include <Sampler.h>
#include <SuperSampler.h>
#include <cstdint>
#include <fstream>
#include <memory>
#include <random>
#include <typedefs.h>
#include <vector>

class Mandelbrot : public Sampler<RGBA> {
public:
  Mandelbrot() = default;
  Mandelbrot(const unsigned int &max_iter, const coordType &bailout_val,
             std::shared_ptr<Gradient<RGBA>> grad)
      : max_iterations(max_iter), bailout_value(bailout_val), gradient(grad) {}
  ~Mandelbrot() override = default;

  RGBA sample(const Complex &p) const override {
    // Implement the Mandelbrot sampling logic here
    // return RGBA::yellow();
    Complex      z(0, 0);
    Complex      c         = p;
    unsigned int curr_iter = 0;
    while (mag2(z) < bailout_value && curr_iter < max_iterations) {
      z = z * z + c;
      curr_iter++;
    }
    if (curr_iter >= max_iterations) { return RGBA(40, 10, 120); }
    return gradient->getColor(20.f * float(curr_iter));
  }
  unsigned int                    max_iterations = 1000;
  coordType                       bailout_value  = 4.0;
  std::shared_ptr<Gradient<RGBA>> gradient       = nullptr;
};

class ShittyBoxFilter : public SuperSampler {
public:
  ShittyBoxFilter() = default;
  ShittyBoxFilter(const unsigned int &samples_per_pixel)
      : samples_per_pixel(samples_per_pixel) {}
  ~ShittyBoxFilter() override = default;

  std::vector<Complex> getSamplePoints(const Complex   &p,
                                       const XYMapping &mapping) override {
    std::vector<Complex> sample_points;
    sample_points.reserve(samples_per_pixel);
    for (unsigned int i = 0; i < samples_per_pixel; ++i) {
      sample_points.push_back(
          mapping.getLocation(p + Complex(dist(rng), dist(rng))));
    }
    return sample_points;
  }

private:
  unsigned int                                  samples_per_pixel = 1;
  mutable std::mt19937                          rng{2025};
  mutable std::uniform_real_distribution<float> dist{-0.5f, 0.5f};
};

int main() {
  // Get parameters from JSON file
  std::ifstream  file("params.json");
  nlohmann::json params;
  file >> params;
  file.close();

  // Set the resolution of the image
  meshInt xres = params.at("x_resolution").get<int>();
  meshInt yres = params.at("y_resolution").get<int>();

  // Set the number of samples per pixel
  unsigned int samples_per_pixel = params.at("samples_per_pixel").get<int>();
  // Set the number of iterations
  unsigned int max_iterations = params.at("max_iterations").get<int>();
  // Set the bailout value
  coordType bailout_value = params.at("bailout_value").get<float>();
  // Set the location of the center
  Complex center(params.at("center").at("x").get<float>(),
                 params.at("center").at("y").get<float>());
  // Set the scale
  coordType scale = 1.0f / params.at("zoom").get<float>();
  // Set the rotation
  coordType rotation = params.at("rotation").get<float>();

  // Make a gradient
  std::shared_ptr<Gradient<RGBA>> gradient = std::make_shared<Gradient<RGBA>>();
  gradient->addNode(RGBA::cyan(), 0.0);
  gradient->addNode(RGBA::magenta(), 120.0);
  gradient->addNode(RGBA::yellow(), 240.0);

  // Make a Mandelbrot sampler
  std::shared_ptr<Sampler<RGBA>> sampler =
      std::make_shared<Mandelbrot>(max_iterations, bailout_value, gradient);

  // Identity mapping
  std::shared_ptr<Mapping> identity = std::make_shared<Mapping>();

  // Supersampler
  std::shared_ptr<SuperSampler> super_sampler =
      std::make_shared<ShittyBoxFilter>(samples_per_pixel);

  // Create Canvas
  Canvas<RGBA> canvas(xres, yres, super_sampler);
  canvas.setLocation(center, scale, rotation);
  canvas.addLayer(sampler, identity);

  canvas.draw();

  const uint8_t *imagedata =
      reinterpret_cast<const uint8_t *>(canvas.getImage().getData());

  /* unsigned int mid = (xres * yres) / 2;
  std::cout << "r: " << int(imagedata[mid * 4 + 0]) << " ";
  std::cout << "g: " << int(imagedata[mid * 4 + 1]) << " ";
  std::cout << "b: " << int(imagedata[mid * 4 + 2]) << " ";
  std::cout << "a: " << int(imagedata[mid * 4 + 3]) << " "; */
  /* for (int i = 0; i < xres * yres; ++i) {
    std::cout << "r: [" << int(imagedata[i * 4 + 0]) << ","
              << int(canvas.getImage().getData()[i].r) << "] ";
    std::cout << "g: [" << int(imagedata[i * 4 + 1]) << ","
              << int(canvas.getImage().getData()[i].g) << "] ";
    std::cout << "b: [" << int(imagedata[i * 4 + 2]) << ","
              << int(canvas.getImage().getData()[i].b) << "] ";
    std::cout << "a: [" << int(imagedata[i * 4 + 3]) << ","
              << int(canvas.getImage().getData()[i].a) << "] ";
  } */

  if (stbi_write_png("test1.png", xres, yres, 4, imagedata, 4 * xres)) {
    std::cout << "Image 1 saved!\n";
  } else {
    std::cerr << "Failed to save image.\n";
  }
  return 0;
}
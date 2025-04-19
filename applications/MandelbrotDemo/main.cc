#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <IMWrite/stb_image_write.h>
#include <json/json.hpp>

#include <xRenderer/color/Color.h>
#include <xRenderer/color/Gradient.h>
#include <xRenderer/color/RGBA.h>
#include <xRenderer/core/Canvas.h>
#include <xRenderer/core/Complex.h>
#include <xRenderer/core/typedefs.h>
#include <xRenderer/mappings/Mapping.h>
#include <xRenderer/samplers/Mandelbrot.h>
#include <xRenderer/super_samplers/GaussianFilter.h>

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sys/types.h>

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
  std::shared_ptr<Sampler<RGBA>> sampler = std::make_shared<Mandelbrot<RGBA>>(
      max_iterations, bailout_value, gradient);

  // Identity mapping
  std::shared_ptr<Mapping> identity = std::make_shared<Mapping>();

  // Supersampler
  std::shared_ptr<SuperSampler> super_sampler =
      std::make_shared<GaussianFilter>(samples_per_pixel);

  // Create Canvas
  Canvas<RGBA> canvas(xres, yres, super_sampler);
  canvas.setLocation(center, scale, rotation);
  canvas.addLayer(sampler, identity);

  auto start = std::chrono::high_resolution_clock::now();
  canvas.draw();
  auto                          end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Draw time: " << elapsed.count() << " seconds\n";

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
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <IMWrite/stb_image_write.h>
#include <SFML/Graphics.hpp>
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
#include <thread>

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

  std::string output_file = params.at("output_file").get<std::string>();

  // Make a gradient
  std::shared_ptr<Gradient<RGBA>> gradient = std::make_shared<Gradient<RGBA>>();
  gradient->addNode(RGBA::cyan(), 0.0);
  gradient->addNode(RGBA::magenta(), 120.0);
  gradient->addNode(RGBA::white(), 240.0);

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

  auto        start = std::chrono::high_resolution_clock::now();
  std::thread drawing(&Canvas<RGBA>::draw, &canvas);
  auto        end = std::chrono::high_resolution_clock::now();
  // std::chrono::duration<double> elapsed = end - start;
  // std::cout << "Draw time: " << elapsed.count() << " seconds\n";

  sf::RenderWindow window(sf::VideoMode(xres, yres), "Real-Time Grid");
  sf::Texture      texture;
  texture.create(xres, yres);
  sf::Sprite sprite(texture);

  std::vector<sf::Uint8> pixels(xres * yres * 4); // RGBA

  // std::thread printing([&]() {
  //   while (window.isOpen() && drawing.joinable()) {
  //     sf::Event event;
  //     while (window.pollEvent(event)) {
  //       if (event.type == sf::Event::Closed) window.close();
  //     }
  //
  //    texture.update(
  //        reinterpret_cast<const uint8_t *>(canvas.getImage().getData()));
  //    // window.clear();
  //    window.draw(sprite);
  //    window.display();
  //  }
  //}
  //                     //, std::ref(canvas), std::ref(texture),
  //                     std::ref(window)
  //);

  while (window.isOpen() && drawing.joinable()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    texture.update(
        reinterpret_cast<const uint8_t *>(canvas.getImage().getData()));
    // window.clear();
    window.draw(sprite);
    window.display();
  }

  drawing.join();
  // printing.join();

  const uint8_t *imagedata =
      reinterpret_cast<const uint8_t *>(canvas.getImage().getData());

  if (stbi_write_png(output_file.c_str(), xres, yres, 4, imagedata, 4 * xres)) {
    std::cout << "Image 1 saved!\n";
  } else {
    std::cerr << "Failed to save image.\n";
  }
  return 0;
}
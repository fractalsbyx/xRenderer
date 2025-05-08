#include <memory>
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
#include <xRenderer/samplers/GnarlBase.h>
#include <xRenderer/super_samplers/GaussianFilter.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <thread>

int
main() {
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
  realType time = params.at("time").get<double>();
  realType dt   = params.at("dt").get<double>();
  // Set color speed
  float color_speed = params.at("color_speed").get<float>();
  float color_shift = params.at("color_shift").get<float>();
  // Set the location of the center
  Complex center(params.at("center").at("x").get<float>(),
                 params.at("center").at("y").get<float>());
  // Set the scale
  realType scale = 1.0f / params.at("zoom").get<float>();
  // Set the rotation
  realType rotation = params.at("rotation").get<float>();

  std::string output_file = params.at("output_file").get<std::string>();

  // Make a gradient
  std::shared_ptr<Gradient<RGBA>> gradient = std::make_shared<Gradient<RGBA>>();
  gradient->addNode(RGBA::cyan(), 0.0);
  gradient->addNode(RGBA::magenta(), 120.0);
  gradient->addNode(RGBA::white(), 240.0);
  gradient->speed = color_speed;
  gradient->shift = color_shift;

  // Define a Gnarl generator
  class SquareGnarl : public GnarlGenerator {
  public:
    Dual2D
    generator(const Dual2D &x, const Dual2D &y) override {
      return cos(x) * cos(y);
    }
  };

  // Create a square generator
  std::shared_ptr<SquareGnarl> square_generator =
      std::make_shared<SquareGnarl>();

  // Make a Gnarl sampler
  std::shared_ptr<Sampler<RGBA>> sampler =
      std::make_shared<GnarlBase<RGBA>>(time, dt, square_generator, gradient);

  // Supersampler
  std::shared_ptr<SuperSampler> super_sampler =
      std::make_shared<GaussianFilter>(samples_per_pixel);

  // Create Canvas
  Canvas<RGBA> canvas(xres, yres, super_sampler);
  canvas.setLocation(center, scale, rotation);
  canvas.addLayer(sampler);

  std::thread drawing(&Canvas<RGBA>::draw, &canvas);

  sf::RenderWindow window(sf::VideoMode(xres, yres), "Real-Time Grid");
  sf::Texture      texture;
  texture.create(xres, yres);
  sf::Sprite sprite(texture);

  std::vector<sf::Uint8> pixels(xres * yres * 4); // RGBA

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
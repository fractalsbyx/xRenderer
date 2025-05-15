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
  realType time            = params.at("time").get<double>();
  realType dt              = params.at("dt").get<double>();
  realType circle_fraction = params.at("circle_fraction").get<double>();

  unsigned int sym     = params.at("symmetry").get<int>();
  realType     scale1  = params.at("scale1").get<double>();
  realType     weight1 = params.at("weight1").get<double>();
  realType     scale2  = params.at("scale2").get<double>();
  realType     weight2 = params.at("weight2").get<double>();

  // Set color speed
  float color_speed = params.at("color_speed").get<float>();
  float color_shift = params.at("color_shift").get<float>();

  float layer2_opacity = params.at("layer2_opacity").get<float>();

  // Set the location of the center
  Complex center(params.at("center").at("x").get<float>(),
                 params.at("center").at("y").get<float>());
  // Set the scale
  realType scale = 1.0f / params.at("zoom").get<float>();
  // Set the rotation
  realType rotation = params.at("rotation").get<float>();

  std::string output_file = params.at("output_file").get<std::string>();

  // Make a gradient
  std::shared_ptr<Gradient<RGBA>> gradient1 =
      std::make_shared<Gradient<RGBA>>();
  gradient1->addNode(RGBA::cyan(), 0.0);
  gradient1->addNode(RGBA::magenta(), 120.0);
  gradient1->addNode(RGBA::white(), 240.0);
  gradient1->speed = color_speed;
  gradient1->shift = color_shift;

  std::shared_ptr<Gradient<RGBA>> gradient2 =
      std::make_shared<Gradient<RGBA>>();
  gradient2->addNode(RGBA::cyan(), 0.0);
  gradient2->addNode(RGBA::yellow(), 120.0);
  gradient2->addNode(RGBA::white(), 240.0);
  gradient2->speed = color_speed;
  gradient2->shift = color_shift;

  // Define a Gnarl generator
  class SquareGnarl : public GnarlGenerator {
  public:
    Dual2D
    generator(const Dual2D &x, const Dual2D &y) const override {
      return cos(x) * cos(y);
    }
  };
  // Define a Gnarl generator
  class FunGnarl : public GnarlGenerator {
  public:
    // Constructor
    FunGnarl(unsigned int _sym = 5, realType _scale1 = 1.0,
             realType _weight1 = 1.0, realType _scale2 = 0.15,
             realType _weight2 = 0.1)
        : sym(_sym), scale1(_scale1), weight1(_weight1), scale2(_scale2),
          weight2(_weight2) {}

    Dual2D
    generator(const Dual2D &x, const Dual2D &y) const override {
      return weight1 * symwaves(x, y, scale1) +
             weight2 * symwaves(x, y, scale2);
    }

    Dual2D
    symwaves(const Dual2D &x, const Dual2D &y, const realType &sc) const {
      // Symmetric waves
      Dual2D result = Dual2D(0.0, Vector2D(0.0, 0.0));
      for (unsigned int i = 1; i <= sym; ++i) {
        result += cos(M_PI * rotate(x, y, i * M_PI / sym).first / sc);
      }
      return result;
    }

    std::pair<Dual2D, Dual2D>
    rotate(const Dual2D &x, const Dual2D &y, const realType &angle) const {
      // Rotate the coordinates by the given angle
      return std::make_pair(std::cos(angle) * x - std::sin(angle) * y,
                            std::sin(angle) * x + std::cos(angle) * y);
    }

    // Parameters for the generator
    unsigned int sym;     // Number of symmetric waves
    realType     scale1;  // Scale for the first wave
    realType     weight1; // Weight for the first wave
    realType     scale2;  // Scale for the second wave
    realType     weight2; // Weight for the second wave
  };

  // Create a generator
  std::shared_ptr<GnarlGenerator> fun_generator =
      std::make_shared<FunGnarl>(sym, scale1, weight1, scale2, weight2);

  // Make a Gnarl sampler
  std::shared_ptr<Sampler<RGBA>> sampler1 = std::make_shared<GnarlBase<RGBA>>(
      time, dt, (2.0 * M_PI * circle_fraction), true, fun_generator, gradient1);

  std::shared_ptr<Sampler<RGBA>> sampler2 = std::make_shared<GnarlBase<RGBA>>(
      time, dt, (2.0 * M_PI * circle_fraction), true, fun_generator, gradient2,
      std::make_shared<ImagSampler>());

  // Supersampler
  std::shared_ptr<SuperSampler> super_sampler =
      std::make_shared<GaussianFilter>(samples_per_pixel);

  // Create Canvas
  Canvas<RGBA> canvas(xres, yres, super_sampler);
  canvas.setLocation(center, scale, rotation);
  canvas.addLayer(sampler1);
  canvas.addLayer(sampler2);
  canvas.layers[1].setOpacity(layer2_opacity);

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
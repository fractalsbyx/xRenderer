#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

int main() {
  const int width  = 200;
  const int height = 200;

  sf::RenderWindow window(sf::VideoMode(width, height), "Real-Time Grid");
  sf::Texture      texture;
  texture.create(width, height);
  sf::Sprite sprite(texture);

  std::vector<sf::Uint8> pixels(width * height * 4); // RGBA

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    // Generate/update pixel data
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        float value =
            std::sin((x + y + clock()) * 0.0001f); // your grid data here
        sf::Uint8 color = static_cast<sf::Uint8>((value + 1) * 127);
        int       idx   = 4 * (y * width + x);
        pixels[idx + 0] = color;       // R
        pixels[idx + 1] = 0;           // G
        pixels[idx + 2] = 255 - color; // B
        pixels[idx + 3] = 255;         // A
      }
    }

    texture.update(pixels.data());
    window.clear();
    window.draw(sprite);
    window.display();
  }

  return 0;
}
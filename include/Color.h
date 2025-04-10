#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <sys/types.h>

struct Color {
  constexpr Color() {}
  constexpr Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;

  static constexpr Color red() { return Color(255, 0, 0); }
  static constexpr Color green() { return Color(0, 255, 0); }
  static constexpr Color blue() { return Color(0, 0, 255); }
  static constexpr Color cyan() { return Color(0, 255, 255); }
  static constexpr Color magenta() { return Color(255, 0, 255); }
  static constexpr Color yellow() { return Color(255, 255, 0); }
  static constexpr Color black() { return Color(0, 0, 0); }
  static constexpr Color white() { return Color(255, 255, 255); }

  static Color interpolate(const Color &c1, const Color &c2, float fraction) {
    // Implement the interpolation logic here
    return Color();
  }
};

#endif // COLOR_H
#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <sys/types.h>

enum class MixingMode {
  NORMAL,
  ADDITIVE,
  MULTIPLY,
  SCREEN,
  OVERLAY,
  DARKEN,
  LIGHTEN,
  COLOR_BURN,
  COLOR_DODGE,
  HARD_LIGHT,
  SOFT_LIGHT
};

// template <typename ColorSpace> struct Color {
//   constexpr Color() {}
//
//   ColorSpace color_object;
//
//   static constexpr Color red() { return Color(255, 0, 0); }
//   static constexpr Color green() { return Color(0, 255, 0); }
//   static constexpr Color blue() { return Color(0, 0, 255); }
//   static constexpr Color cyan() { return Color(0, 255, 255); }
//   static constexpr Color magenta() { return Color(255, 0, 255); }
//   static constexpr Color yellow() { return Color(255, 255, 0); }
//   static constexpr Color black() { return Color(0, 0, 0); }
//   static constexpr Color white() { return Color(255, 255, 255); }
//
//   static ColorSpace interpolate(const Color &c1, const Color &c2,
//                                 float fraction_c2) {
//     // Implement the interpolation logic here
//     return ColorSpace::interpolate(c1.color_object, c2.color_object,
//                                    fraction_c2);
//   }
//
// };

#endif // COLOR_H
#ifndef COLORSPACES_H
#define COLORSPACES_H

#include <algorithm>
#include <cstdint>

#include <xRenderer/color/Color.h>

using mixInt = unsigned int;

#pragma pack(push, 1)
struct RGBA {
  constexpr RGBA() {}
  constexpr RGBA(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255)
      : r(_r), g(_g), b(_b), a(_a) {}

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t a = 255;

  static constexpr RGBA red() { return RGBA(255, 0, 0); }
  static constexpr RGBA green() { return RGBA(0, 255, 0); }
  static constexpr RGBA blue() { return RGBA(0, 0, 255); }
  static constexpr RGBA cyan() { return RGBA(0, 255, 255); }
  static constexpr RGBA magenta() { return RGBA(255, 0, 255); }
  static constexpr RGBA yellow() { return RGBA(255, 255, 0); }
  static constexpr RGBA black() { return RGBA(0, 0, 0); }
  static constexpr RGBA white() { return RGBA(255, 255, 255); }
  static constexpr RGBA blank() { return RGBA(0, 0, 0, 0); }

  static RGBA interpolate(const RGBA &c1, const RGBA &c2, float fraction_c2) {
    // Implement the interpolation logic here
    return RGBA((1.0 - fraction_c2) * c1.r + fraction_c2 * c2.r,
                (1.0 - fraction_c2) * c1.g + fraction_c2 * c2.g,
                (1.0 - fraction_c2) * c1.b + fraction_c2 * c2.b,
                (1.0 - fraction_c2) * c1.a + fraction_c2 * c2.a);
  }

  template <typename container> static RGBA blend(const container &samples) {
    mixInt num_samples = samples.size();
    if (num_samples == 0) { return RGBA::blank(); }
    mixInt blended_r = 0;
    mixInt blended_g = 0;
    mixInt blended_b = 0;
    mixInt blended_a = 0;
    for (const auto &sample : samples) {
      blended_r += sample.r;
      blended_g += sample.g;
      blended_b += sample.b;
      blended_a += sample.a;
    }
    return RGBA(
        uint8_t(blended_r / num_samples), uint8_t(blended_g / num_samples),
        uint8_t(blended_b / num_samples), uint8_t(blended_a / num_samples));
  }

  static RGBA applyLayer(const RGBA &base, const RGBA &layer,
                         const float &opacity, MixingMode mixing_mode) {
    switch (mixing_mode) {
    case MixingMode::NORMAL:
      return RGBA(
          (base.r * (1.0 - opacity) * base.a / 255.f +
           layer.r * opacity * layer.a / 255.f),
          (base.g * (1.0 - opacity) * base.a / 255.f +
           layer.g * opacity * layer.a / 255.f),
          (base.b * (1.0 - opacity) * base.a / 255.f +
           layer.b * opacity * layer.a / 255.f),
          uint8_t(std::min(mixInt(base.a) + mixInt(layer.a * opacity), 255u)));
    case MixingMode::ADDITIVE:
      return RGBA(
          std::min(mixInt(base.r) + mixInt(layer.r), 255u),
          std::min(mixInt(base.g) + mixInt(layer.g), 255u),
          std::min(mixInt(base.b) + mixInt(layer.b), 255u),
          uint8_t(std::min(mixInt(base.a) + mixInt(layer.a * opacity), 255u)));
    // Add other mixing modes as needed
    default:
      return base;
    }
  }
};
#pragma pack(pop)

static_assert(sizeof(RGBA) == 4, "RGBA struct must be tightly packed");
static_assert(std::is_trivially_copyable<RGBA>::value,
              "RGBA must be trivially copyable");

#endif // COLORSPACES_H
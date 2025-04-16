#ifndef COLORSPACES_H
#define COLORSPACES_H

#include <algorithm>
#include <cstdint>

#include "Color.h"
using mixInt = unsigned int;
struct RGBA {
  constexpr RGBA() {}
  constexpr RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha = 255)
      : r(r), g(g), b(b), alpha(alpha) {}

  uint8_t r     = 0;
  uint8_t g     = 0;
  uint8_t b     = 0;
  uint8_t alpha = 255;

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
                (1.0 - fraction_c2) * c1.alpha + fraction_c2 * c2.alpha);
  }

  template <typename container> static RGBA blend(const container &samples) {
    mixInt num_samples = samples.size();
    if (num_samples == 0) { return RGBA(); }
    mixInt blended_r     = 0;
    mixInt blended_g     = 0;
    mixInt blended_b     = 0;
    mixInt blended_alpha = 0;
    for (const auto &sample : samples) {
      blended_r += sample.r;
      blended_g += sample.g;
      blended_b += sample.b;
      blended_alpha += sample.alpha;
    }
    return RGBA(
        uint8_t(blended_r / num_samples), uint8_t(blended_g / num_samples),
        uint8_t(blended_b / num_samples), uint8_t(blended_alpha / num_samples));
  }

  static RGBA applyLayer(const RGBA &base, const RGBA &layer,
                         const uint8_t &opacity, MixingMode mixing_mode) {
    switch (mixing_mode) {
    case MixingMode::NORMAL:
      return RGBA(
          (base.r * (1.0 - opacity) + layer.r * opacity),
          (base.g * (1.0 - opacity) + layer.g * opacity),
          (base.b * (1.0 - opacity) + layer.b * opacity),
          uint8_t(std::min(mixInt(base.alpha) + mixInt(layer.alpha) * opacity,
                           255u)));
    case MixingMode::ADDITIVE:
      return RGBA(
          std::min(mixInt(base.r) + mixInt(layer.r), 255u),
          std::min(mixInt(base.g) + mixInt(layer.g), 255u),
          std::min(mixInt(base.b) + mixInt(layer.b), 255u),
          uint8_t(std::min(mixInt(base.alpha) + mixInt(layer.alpha) * opacity,
                           255u)));
    // Add other mixing modes as needed
    default:
      return base;
    }
  }
};

#endif // COLORSPACES_H
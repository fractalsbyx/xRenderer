#ifndef COLORSPACES_H
#define COLORSPACES_H

#include <algorithm>
#include <cstdint>
#include <unordered_map>

#include <xRenderer/color/Color.h>

using mixInt = unsigned int;

struct RGBA {
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t a = 255;

  constexpr RGBA() {}
  constexpr RGBA(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255)
      : r(_r), g(_g), b(_b), a(_a) {}

  bool operator==(const RGBA &other) const {
    return r == other.r && g == other.g && b == other.b && a == other.a;
  }
  bool operator!=(const RGBA &other) const { return !(*this == other); }

  static constexpr RGBA red();
  static constexpr RGBA green();
  static constexpr RGBA blue();
  static constexpr RGBA cyan();
  static constexpr RGBA magenta();
  static constexpr RGBA yellow();
  static constexpr RGBA black();
  static constexpr RGBA white();
  static constexpr RGBA blank();

  static RGBA interpolate(const RGBA &c1, const RGBA &c2, float fraction_c2);
  template <typename container> static RGBA blend(const container &samples);
  static RGBA blend(const std::unordered_map<RGBA, unsigned int> &histogram,
                    const unsigned int                           &num_samples);
  static RGBA applyLayer(const RGBA &base, const RGBA &layer,
                         const float &opacity, MixingMode mixing_mode);
};

static_assert(sizeof(RGBA) == sizeof(int32_t), "int and RGBA must be 4 bytes");
template <> struct std::hash<RGBA> {
  std::size_t operator()(const RGBA &color) const noexcept {

    return std::hash<int32_t>{}(*(reinterpret_cast<const int32_t *>(&color)));
  }
};

constexpr RGBA RGBA::red() { return RGBA(255, 0, 0); }
constexpr RGBA RGBA::green() { return RGBA(0, 255, 0); }
constexpr RGBA RGBA::blue() { return RGBA(0, 0, 255); }
constexpr RGBA RGBA::cyan() { return RGBA(0, 255, 255); }
constexpr RGBA RGBA::magenta() { return RGBA(255, 0, 255); }
constexpr RGBA RGBA::yellow() { return RGBA(255, 255, 0); }
constexpr RGBA RGBA::black() { return RGBA(0, 0, 0); }
constexpr RGBA RGBA::white() { return RGBA(255, 255, 255); }
constexpr RGBA RGBA::blank() { return RGBA(0, 0, 0, 0); }

RGBA RGBA::interpolate(const RGBA &c1, const RGBA &c2, float fraction_c2) {
  // Implement the interpolation logic here
  return RGBA((1.0 - fraction_c2) * c1.r + fraction_c2 * c2.r,
              (1.0 - fraction_c2) * c1.g + fraction_c2 * c2.g,
              (1.0 - fraction_c2) * c1.b + fraction_c2 * c2.b,
              (1.0 - fraction_c2) * c1.a + fraction_c2 * c2.a);
}

template <typename container> RGBA RGBA::blend(const container &samples) {
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

RGBA RGBA::blend(const std::unordered_map<RGBA, unsigned int> &histogram,
                 const unsigned int                           &num_samples) {

  if (num_samples == 0) { return RGBA::blank(); }
  mixInt blended_r = 0;
  mixInt blended_g = 0;
  mixInt blended_b = 0;
  mixInt blended_a = 0;
  for (const auto &sample : histogram) {
    blended_r += sample.first.r * sample.second;
    blended_g += sample.first.g * sample.second;
    blended_b += sample.first.b * sample.second;
    blended_a += sample.first.a * sample.second;
  }
  return RGBA(
      uint8_t(blended_r / num_samples), uint8_t(blended_g / num_samples),
      uint8_t(blended_b / num_samples), uint8_t(blended_a / num_samples));
}

RGBA RGBA::applyLayer(const RGBA &base, const RGBA &layer, const float &opacity,
                      MixingMode mixing_mode) {
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

static_assert(sizeof(RGBA) == 4, "RGBA struct must be tightly packed");
static_assert(std::is_trivially_copyable<RGBA>::value,
              "RGBA must be trivially copyable");

#endif // COLORSPACES_H
#ifndef LAYER_H
#define LAYER_H

#include "Color.h"
#include "Mapping.h"
#include "Sampler.h"
#include "typedefs.h"
#include <algorithm>
#include <cstdint>
#include <memory>

template <typename Color> class Layer {
public:
  Layer(std::shared_ptr<Sampler<Color>> sampler,
        std::shared_ptr<Mapping>        mapping = std::make_shared<Mapping>(),
        MixingMode mixing_mode = MixingMode::NORMAL, float opacity = 1.0,
        bool visible = true)
      : sampler(sampler), mapping(mapping), mixing_mode(mixing_mode),
        opacity(opacity), visible(visible) {}

  bool isVisible() const { return visible; }
  void setVisible(bool v) { visible = v; }

  const float &getOpacity() const { return opacity; }
  void         setOpacity(float o) { std::clamp(o, 0.0f, 1.0f); }

  const MixingMode &getMixingMode() const { return mixing_mode; }
  void              setMixingMode(MixingMode mode) { mixing_mode = mode; }

  void draw() {
    // Implement the drawing logic here
    // This is a placeholder for the actual drawing logic
  }

  Color sample(const Complex &p) const {
    return sampler->sample((*mapping)(p));
  }

private:
  std::shared_ptr<Sampler<Color>> sampler = nullptr; // Sampler for this layer
  std::shared_ptr<Mapping>        mapping = nullptr; // Mapping for this layer
  MixingMode mixing_mode                  = MixingMode::NORMAL; // Mixing mode
  float      opacity                      = 1.0;  // Opacity of the layer
  bool       visible                      = true; // Visibility flag
};

#endif // LAYER_H
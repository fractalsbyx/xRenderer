#ifndef SAMPLER_H
#define SAMPLER_H

#include "Meshpoint.h"
#include "XYMapping.h"
template <typename Color> class Sampler {
public:
  Sampler();
  virtual ~Sampler() = default;

  virtual Color sample(const Meshpoint &p, const XYMapping *mapping) {
    return Color(0, 0, 0);
  }
};

#endif // SAMPLER_H
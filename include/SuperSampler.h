#ifndef SUPERSAMPLER_H
#define SUPERSAMPLER_H

#include "Meshpoint.h"
#include "Sampler.h"
#include "XYMapping.h"
template <typename Color> class SuperSampler {
public:
  SuperSampler();
  virtual ~SuperSampler() = default;

  virtual Color supersample(const Meshpoint &p, const XYMapping *mapping,
                            const Sampler *sampler) {
    return sampler->sample(p, mapping);
  }
};

#endif // SUPERSAMPLER_H
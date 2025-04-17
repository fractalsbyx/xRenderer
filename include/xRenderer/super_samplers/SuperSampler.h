#ifndef SUPERSAMPLER_H
#define SUPERSAMPLER_H

#include <xRenderer/core/Complex.h>
#include <xRenderer/core/Meshpoint.h>
#include <xRenderer/core/XYMapping.h>
#include <xRenderer/core/typedefs.h>

#include <vector>
// class SuperSampler {
// public:
//   SuperSampler();
//   virtual ~SuperSampler() = default;
//
//   Color superSample(const Meshpoint &p, const XYMapping *mapping,
//                     const Sampler<Color> *sampler) const {
//     return this->superSample(p.toComplex(), mapping, sampler);
//   }
//
//   virtual Color superSample(const Complex &p, const XYMapping *mapping,
//                             const Sampler<Color> *sampler) const {
//     return sampler->sample(mapping->getLocation(p));
//   }
// };

class SuperSampler {
public:
  SuperSampler()          = default;
  virtual ~SuperSampler() = default;

  virtual std::vector<Complex> getSamplePoints(const Complex   &p,
                                               const XYMapping &mapping) {
    std::vector<Complex> samplePoints;
    samplePoints.push_back(mapping.getLocation(p));
    return samplePoints;
  }
};

#endif // SUPERSAMPLER_H
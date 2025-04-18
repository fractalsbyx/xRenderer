#ifndef BOXFILTER_H
#define BOXFILTER_H

#include <xRenderer/super_samplers/SuperSampler.h>

#include <random>

class ShittyBoxFilter : public SuperSampler {
public:
  ShittyBoxFilter() = default;
  ShittyBoxFilter(const unsigned int &samples_per_pixel)
      : samples_per_pixel(samples_per_pixel) {}
  ~ShittyBoxFilter() override = default;

  std::vector<Complex> getSamplePoints(const Complex   &p,
                                       const XYMapping &mapping) override {
    std::vector<Complex> sample_points;
    sample_points.reserve(samples_per_pixel);
    for (unsigned int i = 0; i < samples_per_pixel; ++i) {
      sample_points.push_back(
          mapping.getLocation(p + Complex(dist(rng), dist(rng))));
    }
    return sample_points;
  }

private:
  unsigned int                     samples_per_pixel = 1;
  static thread_local std::mt19937 rng;
  static thread_local std::uniform_real_distribution<float> dist;
};

thread_local std::mt19937                          ShittyBoxFilter::rng{2025};
thread_local std::uniform_real_distribution<float> ShittyBoxFilter::dist{-0.5f,
                                                                         0.5f};

#endif // BOXFILTER_H
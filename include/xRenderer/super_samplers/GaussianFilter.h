#ifndef GAUSSIANFILTER_H
#define GAUSSIANFILTER_H

#include <xRenderer/super_samplers/SuperSampler.h>

#include <random>

class GaussianFilter : public SuperSampler {
public:
  GaussianFilter() = default;
  GaussianFilter(const unsigned int &samples_per_pixel = 1,
                 const float        &sigma             = 0.5f)
      : samples_per_pixel(samples_per_pixel), dist(0.0f, sigma) {}
  ~GaussianFilter() override = default;

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
  std::normal_distribution<float>  dist{0.0f, 0.5f};
};

thread_local std::mt19937 GaussianFilter::rng{2025};
// thread_local std::normal_distribution<float> GaussianFilter::dist{0.0f,
// 0.5f};

#endif // GAUSSIANFILTER_H
#ifndef SAMPLER_H
#define SAMPLER_H

template <typename T> class Sampler {
public:
  Sampler()          = default;
  virtual ~Sampler() = default;

  virtual T sample([[maybe_unused]] const Complex &p) const { return T(); }
};

#endif // SAMPLER_H
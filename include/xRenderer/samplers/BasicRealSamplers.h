#ifndef BASICREALSAMPLERS_H
#define BASICREALSAMPLERS_H

#include <xRenderer/core/Complex.h>
#include <xRenderer/core/typedefs.h>
#include <xRenderer/samplers/Sampler.h>

class RealSampler : public Sampler<realType> {
public:
  realType
  sample([[maybe_unused]] const Complex &p) const override {
    return p.real();
  }
};

class ImagSampler : public Sampler<realType> {
public:
  realType
  sample([[maybe_unused]] const Complex &p) const override {
    return p.imag();
  }
};

class AngleSampler : public Sampler<realType> {
public:
  realType
  sample([[maybe_unused]] const Complex &p) const override {
    return p.arg();
  }
};

class MagSampler : public Sampler<realType> {
public:
  realType
  sample([[maybe_unused]] const Complex &p) const override {
    return p.mag();
  }
};

class Mag2Sampler : public Sampler<realType> {
public:
  realType
  sample([[maybe_unused]] const Complex &p) const override {
    return p.mag2();
  }
};

class GaussianInteger : public Sampler<Complex> {
public:
  Complex
  sample([[maybe_unused]] const Complex &p) const override {
    return p - Complex(std::round(p.real()), std::round(p.imag()));
  }
};

#endif // BASICREALSAMPLERS_H
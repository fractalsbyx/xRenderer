#ifndef BASICCOMPLEXSAMPLERS_H
#define BASICCOMPLEXSAMPLERS_H

#include <xRenderer/core/Complex.h>
#include <xRenderer/core/typedefs.h>
#include <xRenderer/samplers/Sampler.h>

class GaussianInteger : public Sampler<Complex> {
public:
  Complex
  sample([[maybe_unused]] const Complex &p) const override {
    return p - Complex(std::round(p.real()), std::round(p.imag()));
  }
};

#endif // BASICCOMPLEXSAMPLERS_H
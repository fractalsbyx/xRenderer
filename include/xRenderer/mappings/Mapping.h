#ifndef MAPPING_H
#define MAPPING_H

#include <xRenderer/core/Complex.h>

class Mapping {
public:
  Mapping()          = default;
  virtual ~Mapping() = default;

  virtual Complex operator()(const Complex &z) const { return z; }
};

#endif // MAPPING_H
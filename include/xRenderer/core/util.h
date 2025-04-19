#ifndef UTIL_H
#define UTIL_H

#include <cmath>

template <typename T>
T periodic_difference(const T &a, const T &b, const T &period) {
  T diff = a - b;
  if (diff > period / 2) {
    diff -= period;
  } else if (diff < -period / 2) {
    diff += period;
  }
  return diff;
}

template <typename T>
T directed_periodic_difference(const T &upper, const T &lower,
                               const T &period) {
  T diff = std::fmod(upper, period) - std::fmod(lower, period);
  if (diff <= 0) { diff += period; }
  return diff;
}

template <typename number> number power(number base, int exp) {
  if (exp == 0) { return 1; }
  if (exp < 0) { return 1.0 / power(base, -exp); }
  if (exp % 2 == 0) {
    number half_power = power(base, exp / 2);
    return half_power * half_power;
  } else {
    return base * power(base, exp - 1);
  }
}

#endif // UTIL_H
#ifndef UTIL_H
#define UTIL_H

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
  T diff = (upper % period) - (lower % period);
  if (diff <= 0) {
    diff += period;
  }
  return diff;
}

#endif // UTIL_H
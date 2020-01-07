#pragma once

#include <stdlib.h>

template <typename T> T Max(const T &a, const T &b) { return (a > b) ? a : b; }

template <typename T> T Min(const T &a, const T &b) { return (a < b) ? a : b; }

template <typename T> T Abs(const T &a) { return (a < T(0)) ? (T(-1) * a) : a; }

inline float Random(float min, float max) {
  const float range = max - min;
  const float random_float =
      static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  return (range * random_float) + min;
}

inline int Random(int min, int max) {
  const int range = max - min;
  return (rand() % range) + min;
}
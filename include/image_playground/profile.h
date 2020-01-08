#pragma once

#include <chrono>

struct Profiler {
  void Start();
  uint64_t EndAndGetElapsedUS();
  void EndAndPrintElapsedUS(const char *label);

  std::chrono::high_resolution_clock::time_point start;
  bool running;
};
#include "image_playground/profile.h"

#include <cassert>
#include <chrono>
#include <stdio.h>

void Profiler::Start() {
  assert(!running);
  running = true;
  start = std::chrono::high_resolution_clock::now();
}

uint64_t Profiler::EndAndGetElapsedUS() {
  using namespace std::chrono;
  const high_resolution_clock::time_point end = high_resolution_clock::now();
  assert(running);
  running = false;

  return duration_cast<microseconds>(end - start).count();
}

void Profiler::EndAndPrintElapsedUS(const char *label) {
  const uint64_t elapsed_us = EndAndGetElapsedUS();
  printf("%s: %lld us\n", label, elapsed_us);
}
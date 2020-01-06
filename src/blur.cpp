#include "image_playground/blur.h"
#include "image_playground/convolve.h"

#include <algorithm>
#include <cmath>
#include <type_traits>
#include <vector>

#define PI 3.14159265359f

namespace {

void PrintImageValues(const FloatImage &image);

int ComputeIndex(int cols, int row, int col) { return (row * cols) + col; }

float GuassianKernelValue(float mid_x, float mid_y, float standard_deviation) {
  constexpr float kDelta = 0.01f;

  const float factor = 1.f / (2.f * PI * standard_deviation);

  float x = mid_x - 0.5f;
  const float start_y = mid_y - 0.5f;
  const float end_x = x + 1.f;
  const float end_y = start_y + 1.f;

  float value = 0.f;
  int samples = 0;

  while (x < end_x) {
    const float x_2 = x * x;
    float y = start_y;

    while (y < end_y) {
      const float y_2 = y * y;
      const float exponent = -1.f * (x_2 + y_2) / (2.f * standard_deviation);
      value += factor * exp(exponent) * kDelta * kDelta;
      y += kDelta;
      samples++;
    }

    x += kDelta;
  }

  return value;
}

template <int KernelWidth>
void MakeKernel(float standard_deviation, Kernel<KernelWidth> &kernel) {
  static_assert(KernelWidth % 2 != 0, "Only odd sized kernels are allowed\n");

  constexpr int kCenterPoint = KernelWidth / 2;

  int index = 0;
  for (int row = 0; row < KernelWidth; row++) {
    for (int col = 0; col < KernelWidth; col++) {
      const float x = static_cast<float>(col - kCenterPoint);
      const float y = static_cast<float>(row - kCenterPoint);
      const float value = GuassianKernelValue(x, y, standard_deviation);
      kernel.values[index++] = value;
    }
  }
}

} // namespace

FloatImage MakeBlurImage(const FloatImage &input) {
  constexpr int kKernelWidth = 21;
  constexpr float kSTD = 8;

  Kernel<kKernelWidth> kernel{};
  MakeKernel<kKernelWidth>(kSTD, kernel);
  return Convolve(input, kernel);
}

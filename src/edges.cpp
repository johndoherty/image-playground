#include "image_playground/edges.h"
#include "image_playground/convolve.h"

#include <algorithm>
#include <cmath>
#include <type_traits>
#include <vector>

#define PI 3.14159265359f

namespace {

void PrintImageValues(const FloatImage &image);

int ComputeIndex(int cols, int row, int col) { return (row * cols) + col; }

int ComputeRank(const FloatImage &input) {
  constexpr double kEpsilon = 0.01;
  FloatImage mutable_input = CopyImage(input);

  const int rows = mutable_input.rows;
  const int cols = mutable_input.cols;
  float *const data = mutable_input.data;

  int rank = 0;
  std::vector<bool> row_selected(rows, false);
  for (int col = 0; col < cols; ++col) {
    int row;
    for (row = 0; row < rows; ++row) {
      if (!row_selected[row] &&
          abs(data[ComputeIndex(cols, row, col)]) > kEpsilon)
        break;
    }

    if (row != rows) {
      rank++;
      row_selected[row] = true;

      for (int p = col + 1; p < cols; ++p) {
        data[ComputeIndex(cols, row, p)] /= data[ComputeIndex(cols, row, col)];
      }

      for (int k = 0; k < rows; ++k) {
        if (k != row && abs(data[ComputeIndex(cols, k, col)]) > kEpsilon) {
          for (int p = col + 1; p < cols; ++p)
            data[ComputeIndex(cols, k, p)] -= data[ComputeIndex(cols, row, p)] *
                                              data[ComputeIndex(cols, k, col)];
        }
      }
    }
  }

  return rank;
}

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
void MakeKernel(float standard_deviation, float *kernel) {
  static_assert(KernelWidth % 2 != 0, "Only odd sized kernels are allowed\n");

  constexpr int kCenterPoint = KernelWidth / 2;

  int index = 0;
  for (int row = 0; row < KernelWidth; row++) {
    for (int col = 0; col < KernelWidth; col++) {
      const float x = static_cast<float>(col - kCenterPoint);
      const float y = static_cast<float>(row - kCenterPoint);
      const float value = GuassianKernelValue(x, y, standard_deviation);
      kernel[index++] = value;
    }
  }
}

void PrintImageValues(const FloatImage &image) {
  int index = 0;
  float sum = 0.f;
  for (int row = 0; row < image.rows; row++) {
    for (int col = 0; col < image.cols; col++) {
      printf("%f, ", image.data[index]);
      sum += image.data[index];
      index++;
    }
    printf("\n");
  }

  printf("Sum: %f\n", sum);
}

} // namespace

FloatImage MakeEdgeImage(const FloatImage &input) {
  constexpr int kKernelWidth = 11;
  constexpr int kKernelSize = kKernelWidth * kKernelWidth;
  constexpr float kInnerSTD = 1;
  constexpr float kOuterSTD = 3;

  float inner_kernel[kKernelSize];
  MakeKernel<kKernelWidth>(kInnerSTD, inner_kernel);

  float outer_kernel[kKernelSize];
  MakeKernel<kKernelWidth>(kOuterSTD, outer_kernel);

  const FloatImage inner = Convolve(input, inner_kernel, kKernelWidth);
  const FloatImage outer = Convolve(input, outer_kernel, kKernelWidth);

  const int image_size = input.rows * input.cols;
  FloatImage result = CopyImage(inner);

  for (int i = 0; i < image_size; i++) {
    result.data[i] -= outer.data[i];
  }

  return result;
}

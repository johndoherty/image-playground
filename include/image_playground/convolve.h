#pragma once

#include "image_playground/image.h"

template <int N> struct Kernel { float values[N * N]; };

template <int N>
FloatImage Convolve(const FloatImage &input, const Kernel<N> &kernel) {
  FloatImage result(input.rows, input.cols);

  constexpr int kKernelHalfWidth = N / 2;
  constexpr int kKernelSize = N * N;

  float image_values[kKernelSize];

  const int rows = input.rows;
  const int cols = input.cols;

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      memset(image_values, 0.f, (N * N * sizeof(float)));

      const int start_col = std::max(col - kKernelHalfWidth, 0);
      const int start_row = std::max(row - kKernelHalfWidth, 0);
      const int end_col = std::min(col + kKernelHalfWidth, cols - 1);
      const int end_row = std::min(row + kKernelHalfWidth, rows - 1);

      // printf("Output: %d, %d. Start: %d, %d. End: %d %d\n", row, col,
      // start_row,
      //        start_col, end_row, end_col);

      const int kernel_start_row = row - kKernelHalfWidth;
      const int kernel_start_col = col - kKernelHalfWidth;
      float out_value = 0.f;

      for (int y = start_row; y <= end_row; y++) {
        for (int x = start_col; x <= end_col; x++) {
          const int image_index = (y * cols) + x;
          const int kernel_row = y - kernel_start_row;
          const int kernel_col = x - kernel_start_col;
          const int kernel_index = (kernel_row * N) + kernel_col;
          out_value += input.data[image_index] * kernel.values[kernel_index];
        }
      }

      const int out_index = (row * cols) + col;
      result.data[out_index] = out_value;
    }
  }

  return result;
}
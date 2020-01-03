#include "image_playground/convolve.h"

#include <algorithm>

FloatImage Convolve(const FloatImage &input, const FloatImage &kernel) {
  FloatImage result(input.rows, input.cols);

  if (kernel.cols != kernel.rows) {
    printf("Kernel should be square");
    return result;
  }

  const int kernel_width = kernel.cols;
  const int kernel_half_width = kernel_width / 2;

  const int rows = input.rows;
  const int cols = input.cols;

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      const int start_x = std::max(col - kernel_half_width, 0);
      const int start_y = std::max(row - kernel_half_width, 0);
      const int end_x = std::min(col + kernel_half_width, cols - 1);
      const int end_y = std::min(row + kernel_half_width, rows - 1);

      float out_value = 0.f;
      int kernel_index = 0;

      for (int y = start_y; y <= end_y; y++) {
        int image_index = (y * cols) + col;
        for (int y = start_x; y <= end_x; y++) {
          out_value += input.data[image_index] * kernel.data[kernel_index];
          kernel_index++;
          image_index++;
        }
      }

      const int out_index = (row * cols) + col;
      result.data[out_index] = out_value;
    }
  }

  return result;
}
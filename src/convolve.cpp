#include "image_playground/convolve.h"
#include "image_playground/math.h"

#include <cassert>
#include <cstring>

FloatImage Convolve(const FloatImage &input, const float *const kernel,
                    const int kernel_width) {
  assert(kernel_width % 2 != 0);

  FloatImage result(input.rows, input.cols);

  const int kernel_half_width = kernel_width / 2;
  const int kernel_size = kernel_width * kernel_width;

  const int rows = input.rows;
  const int cols = input.cols;

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      const int start_col = max(col - kernel_half_width, 0);
      const int start_row = max(row - kernel_half_width, 0);
      const int end_col = min(col + kernel_half_width, cols - 1);
      const int end_row = min(row + kernel_half_width, rows - 1);

      const int kernel_start_row = row - kernel_half_width;
      const int kernel_start_col = col - kernel_half_width;
      float out_value = 0.f;

      for (int y = start_row; y <= end_row; y++) {
        for (int x = start_col; x <= end_col; x++) {
          const int image_index = (y * cols) + x;
          const int kernel_row = y - kernel_start_row;
          const int kernel_col = x - kernel_start_col;
          const int kernel_index = (kernel_row * kernel_width) + kernel_col;
          out_value += input.data[image_index] * kernel[kernel_index];
        }
      }

      const int out_index = (row * cols) + col;
      result.data[out_index] = out_value;
    }
  }

  return result;
}
#include "image_playground/fourier.h"
#include "image_playground/math.h"

#include <cassert>
#include <cmath>
#include <stdio.h>

#define PI 3.14159265359f

namespace {

void FourierTransformSeperable(const FloatImage &image, FloatImage &real,
                               FloatImage &imaginary) {
  assert(real.rows == image.rows && real.cols == image.cols);
  assert(imaginary.rows == image.rows && imaginary.cols == image.cols);

  FloatImage real_col_fourier(image.rows, image.cols);
  FloatImage imaginary_col_fourier(image.rows, image.cols);

  const float rows_float = static_cast<float>(image.rows);
  const float cols_float = static_cast<float>(image.cols);

  for (int row = 0; row < image.rows; row++) {
    for (int output_col = 0; output_col < image.cols; output_col++) {
      float real_sum = 0.f;
      float imaginary_sum = 0.f;
      const float output_col_float = static_cast<float>(output_col);

      for (int input_col = 0; input_col < image.cols; input_col++) {
        const float input_col_float = static_cast<float>(input_col);
        const int input_index = (row * image.cols) + input_col;
        const float input_value = image.data[input_index];
        const float exponent =
            -2.f * PI * input_col_float * output_col_float / cols_float;
        real_sum += input_value * cos(exponent);
        imaginary_sum += input_value * sin(exponent);
      }

      const int output_index = (row * image.cols) + output_col;
      real_col_fourier.data[output_index] = real_sum;
      imaginary_col_fourier.data[output_index] = imaginary_sum;
    }
  }

  // TODO: Do a transpose
  for (int col = 0; col < image.cols; col++) {
    for (int output_row = 0; output_row < image.rows; output_row++) {
      float real_sum = 0.f;
      float imaginary_sum = 0.f;

      for (int input_row = 0; input_row < image.rows; input_row++) {
        const int input_index = (input_row * image.cols) + col;
        const float real_input_value = real_col_fourier.data[input_index];
        const float imaginary_input_value =
            imaginary_col_fourier.data[input_index];
        const float exponent = -2.f * PI * input_row * output_row / rows_float;

        const float cos_exponent = cos(exponent);
        const float sin_exponent = sin(exponent);

        real_sum += (real_input_value * cos_exponent) -
                    (imaginary_input_value * sin_exponent);
        imaginary_sum += (real_input_value * sin_exponent) +
                         (imaginary_input_value * cos_exponent);
      }

      const int output_index = (output_row * image.cols) + col;

      const float real_value = real_sum;
      const float imaginary_value = imaginary_sum;

      real.data[output_index] = real_value;
      imaginary.data[output_index] = imaginary_value;
    }
  }
}

void InverseFourierTransformSeperable(const FloatImage &real_input,
                                      const FloatImage &imaginary_input,
                                      FloatImage &real_output,
                                      FloatImage &imaginary_output) {
  assert(real_input.rows == imaginary_input.rows &&
         real_input.cols == imaginary_input.cols);
  assert(real_output.rows == real_input.rows &&
         real_output.cols == real_input.cols);
  assert(imaginary_output.rows == real_output.rows &&
         imaginary_output.cols == real_output.cols);

  const int rows = real_input.rows;
  const int cols = real_input.cols;
  const int size = rows * cols;

  const float rows_float = static_cast<float>(rows);
  const float cols_float = static_cast<float>(cols);

  FloatImage real_col_fourier(rows, cols);
  FloatImage imaginary_col_fourier(rows, cols);

  for (int row = 0; row < rows; row++) {
    for (int output_col = 0; output_col < cols; output_col++) {
      float real_sum = 0.f;
      float imaginary_sum = 0.f;
      const float output_col_float = static_cast<float>(output_col);

      for (int input_col = 0; input_col < cols; input_col++) {
        const int input_index = (row * cols) + input_col;
        const float real_input_value = real_input.data[input_index];
        const float imaginary_input_value = imaginary_input.data[input_index];

        const float input_col_float = static_cast<float>(input_col);
        const float exponent =
            2.f * PI * input_col_float * output_col_float / cols_float;

        const float cos_exponent = cos(exponent);
        const float sin_exponent = sin(exponent);

        real_sum += (real_input_value * cos_exponent) -
                    (imaginary_input_value * sin_exponent);
        imaginary_sum += (real_input_value * sin_exponent) +
                         (imaginary_input_value * cos_exponent);
      }

      const int output_index = (row * cols) + output_col;
      real_col_fourier.data[output_index] = real_sum / cols_float;
      imaginary_col_fourier.data[output_index] = imaginary_sum / cols_float;
    }
  }

  // TODO: Do a transpose
  for (int col = 0; col < cols; col++) {
    for (int output_row = 0; output_row < rows; output_row++) {
      float real_sum = 0.f;
      float imaginary_sum = 0.f;

      for (int input_row = 0; input_row < rows; input_row++) {
        const int input_index = (input_row * cols) + col;
        const float real_input_value = real_col_fourier.data[input_index];
        const float imaginary_input_value =
            imaginary_col_fourier.data[input_index];
        const float exponent = 2.f * PI * input_row * output_row / rows_float;

        const float cos_exponent = cos(exponent);
        const float sin_exponent = sin(exponent);

        real_sum += (real_input_value * cos_exponent) -
                    (imaginary_input_value * sin_exponent);
        imaginary_sum += (real_input_value * sin_exponent) +
                         (imaginary_input_value * cos_exponent);
      }

      const int output_index = (output_row * cols) + col;

      const float real_value = real_sum;
      const float imaginary_value = imaginary_sum;

      real_output.data[output_index] = real_value / rows_float;
      imaginary_output.data[output_index] = imaginary_value / rows_float;
    }
  }
}

void NormalizeImageRange(FloatImage &image) {
  const int size = image.rows * image.cols;

  float max = std::numeric_limits<float>::lowest();
  float min = std::numeric_limits<float>::max();

  for (int i = 0; i < size; i++) {
    const float value = image.data[i];
    max = Max(max, value);
    min = Min(min, value);
  }

  const float range = max - min;
  const float scale = 1.f / range;

  for (int i = 0; i < size; i++) {
    const float value = image.data[i];
    const float scaled = (value - min) * scale;
    image.data[i] = scaled;
  }
}

} // namespace

void FourierTransform(const FloatImage &image, FloatImage &real,
                      FloatImage &imaginary) {
  FourierTransformSeperable(image, real, imaginary);
}

FloatImage InverseFourierTransform(const FloatImage &real,
                                   const FloatImage &imaginary) {

  FloatImage real_result(real.rows, real.cols);
  FloatImage imaginary_result(imaginary.rows, imaginary.cols);
  InverseFourierTransformSeperable(real, imaginary, real_result,
                                   imaginary_result);
  return real_result;
}

FloatImage VisualizationOutput(const FloatImage &real_fourier,
                               const FloatImage &imaginary_fourier) {
  const int rows = real_fourier.rows;
  const int cols = real_fourier.cols;
  const int size = rows * cols;

  FloatImage magnitude_result(rows, cols);

  for (int i = 0; i < size; i++) {
    const float real_sq = real_fourier.data[i] * real_fourier.data[i];
    const float img_sq = imaginary_fourier.data[i] * imaginary_fourier.data[i];
    const float mag = sqrt(real_sq + img_sq);
    magnitude_result.data[i] = mag;
  }

  NormalizeImageRange(magnitude_result);

  for (int i = 0; i < size; i++) {
    magnitude_result.data[i] = log((magnitude_result.data[i] * 255.f) + 1.f);
  }

  NormalizeImageRange(magnitude_result);

  FloatImage recentered_result(rows, cols);
  for (int i = 0; i < size; i++) {
    recentered_result.data[i] = 0.f;
  }

  const int center_row = rows / 2;
  const int center_col = cols / 2;

  for (int original_row = 0; original_row < rows / 2; original_row++) {
    for (int original_col = 0; original_col < cols / 2; original_col++) {
      const int mapped_row_1 = center_row + (original_row);
      const int mapped_row_2 = center_row - (original_row);
      const int mapped_col_1 = center_row + (original_col);
      const int mapped_col_2 = center_row - (original_col);

      const int index_1 = (mapped_row_1 * cols) + mapped_col_1;
      const int index_2 = (mapped_row_1 * cols) + mapped_col_2;
      const int index_3 = (mapped_row_2 * cols) + mapped_col_1;
      const int index_4 = (mapped_row_2 * cols) + mapped_col_2;

      const int original_index = (original_row * cols) + original_col;
      const float original = magnitude_result.data[original_index];

      recentered_result.data[index_1] = original;
      recentered_result.data[index_2] = original;
      recentered_result.data[index_3] = original;
      recentered_result.data[index_4] = original;
    }
  }

  NormalizeImageRange(magnitude_result);

  return recentered_result;
}
#include "image_playground/fourier.h"
#include "image_playground/math.h"

#include <cassert>
#include <cmath>

#define PI 3.14159265359f

void FourierTransform(const FloatImage &image, FloatImage &real,
                      FloatImage &imaginary) {
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
      real_col_fourier.data[output_index] = real_sum / cols_float;
      imaginary_col_fourier.data[output_index] = imaginary_sum / cols_float;
    }
  }

  assert(real.rows == image.rows && real.cols == image.cols);
  assert(imaginary.rows == image.rows && imaginary.cols == image.cols);

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

      const float real_value = real_sum / rows_float;
      const float imaginary_value = imaginary_sum / rows_float;

      real.data[output_index] = real_value;
      imaginary.data[output_index] = imaginary_value;
    }
  }
}

void ScaleImageRange(FloatImage &image) {
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

FloatImage FourierTransformForVisualizing(const FloatImage &image) {
  FloatImage real_result(image.rows, image.cols);
  FloatImage imaginary_result(image.rows, image.cols);

  FourierTransform(image, real_result, imaginary_result);

  FloatImage magnitude_result(image.rows, image.cols);

  const int size = image.rows * image.cols;

  for (int i = 0; i < size; i++) {
    const float real_sq = real_result.data[i] * real_result.data[i];
    const float img_sq = imaginary_result.data[i] * imaginary_result.data[i];
    const float mag = sqrt(real_sq + img_sq);
    magnitude_result.data[i] = mag;
  }

  ScaleImageRange(magnitude_result);

  for (int i = 0; i < size; i++) {
    magnitude_result.data[i] = log((magnitude_result.data[i] * 255.f) + 1.f);
  }

  ScaleImageRange(magnitude_result);

  FloatImage recentered_result(image.rows, image.cols);
  for (int i = 0; i < size; i++) {
    recentered_result.data[i] = 0.f;
  }

  const int center_row = image.rows / 2;
  const int center_col = image.cols / 2;

  for (int original_row = 0; original_row < image.rows / 2; original_row++) {
    for (int original_col = 0; original_col < image.cols / 2; original_col++) {
      const int mapped_row_1 = center_row + (original_row);
      const int mapped_row_2 = center_row - (original_row);
      const int mapped_col_1 = center_row + (original_col);
      const int mapped_col_2 = center_row - (original_col);

      const int index_1 = (mapped_row_1 * image.cols) + mapped_col_1;
      const int index_2 = (mapped_row_1 * image.cols) + mapped_col_2;
      const int index_3 = (mapped_row_2 * image.cols) + mapped_col_1;
      const int index_4 = (mapped_row_2 * image.cols) + mapped_col_2;

      const int original_index = (original_row * image.cols) + original_col;
      const float original = magnitude_result.data[original_index];

      recentered_result.data[index_1] = original;
      recentered_result.data[index_2] = original;
      recentered_result.data[index_3] = original;
      recentered_result.data[index_4] = original;
    }
  }

  ScaleImageRange(magnitude_result);

  return recentered_result;
}
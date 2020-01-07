#include <stdio.h>
#include <stdlib.h>

#include "image_playground/file_helpers.h"
#include "image_playground/fourier.h"
#include "image_playground/gray_scale.h"
#include "image_playground/image.h"
#include "image_playground/math.h"

static constexpr float kEpsilon = 0.001f;

const char *test_image_name = "assets/cln1.jpg";

int main(int argc, char *argv[]) {
  const std::filesystem::path test_image_path =
      GetImagePlaygroundRoot() / "examples" / test_image_name;

  const std::filesystem::path output_directory =
      GetImagePlaygroundRoot() / "examples" / "outputs";

  std::optional<RGBImage> image_optional = RGBImageFromFile(test_image_path);

  if (!image_optional) {
    return 1;
  }

  const RGBImage image = std::move(*image_optional);
  const FloatImage gray = MakeGrayScaleImage(image);

  FloatImage fourier_real(image.rows, image.cols);
  FloatImage fourier_imaginary(image.rows, image.cols);
  FourierTransform(gray, fourier_real, fourier_imaginary);

  const FloatImage inverse_fourier =
      InverseFourierTransform(fourier_real, fourier_imaginary);

  WriteImageToPNG(inverse_fourier, output_directory / "inverse_fourier.png");

  const int image_size = image.rows * image.cols;

  int error_count = 0;
  for (int i = 0; i < image_size; i++) {
    if (AbsDiff(inverse_fourier.data[i], gray.data[i]) > kEpsilon) {
      error_count++;
    }
  }

  printf("Errors: %d / %d\n", error_count, image_size);

  return 0;
}

void GTFourierTransform(const FloatImage &image, FloatImage &real,
                        FloatImage &imaginary) {
  assert(real.rows == image.rows && real.cols == image.cols);
  assert(imaginary.rows == image.rows && imaginary.cols == image.cols);

  const float rows_float = static_cast<float>(image.rows);
  const float cols_float = static_cast<float>(image.cols);

  for (int output_row = 0; output_row < image.rows; output_row++) {
    printf("slow fourier row: %d / %d\n", output_row, image.rows);
    const float output_row_float = static_cast<float>(output_row);
    for (int output_col = 0; output_col < image.cols; output_col++) {
      const float output_col_float = static_cast<float>(output_col);

      float real_sum = 0.f;
      float imaginary_sum = 0.f;

      for (int input_row = 0; input_row < image.rows; input_row++) {
        const float input_row_float = static_cast<float>(input_row);
        for (int input_col = 0; input_col < image.cols; input_col++) {
          const float input_col_float = static_cast<float>(input_col);
          const int input_index = (input_row * image.cols) + input_col;
          const float input_value = image.data[input_index];

          const float output_input_row =
              (output_row_float * input_row_float) / rows_float;
          const float output_input_col =
              output_col_float * input_col_float / cols_float;

          const float exponent =
              -2.f * PI * (output_input_row + output_input_col);
          real_sum += input_value * cos(exponent);
          imaginary_sum += input_value * sin(exponent);
        }
      }

      const int output_index = (output_row * image.cols) + output_col;
      real.data[output_index] = real_sum;
      imaginary.data[output_index] = imaginary_sum;
    }
  }
}

void GTInverseFourierTransform(const FloatImage &real,
                               const FloatImage &imaginary,
                               FloatImage &real_result,
                               FloatImage &imaginary_result) {
  assert(real.rows == imaginary.rows && real.cols == imaginary.cols);
  assert(real.rows == real_result.rows && real.cols == real_result.cols);
  assert(imaginary_result.rows == real_result.rows &&
         imaginary_result.cols == real_result.cols);

  const int rows = real.rows;
  const int cols = real.cols;
  const int size = rows * cols;

  const float rows_float = static_cast<float>(rows);
  const float cols_float = static_cast<float>(cols);
  const float size_float = static_cast<float>(size);

  for (int output_row = 0; output_row < rows; output_row++) {
    printf("slow inverse fourier row: %d / %d\n", output_row, rows);
    const float output_row_float = static_cast<float>(output_row);
    for (int output_col = 0; output_col < cols; output_col++) {
      const float output_col_float = static_cast<float>(output_col);

      float real_sum = 0.f;
      float imaginary_sum = 0.f;

      for (int input_row = 0; input_row < rows; input_row++) {
        const float input_row_float = static_cast<float>(input_row);
        for (int input_col = 0; input_col < cols; input_col++) {
          const float input_col_float = static_cast<float>(input_col);

          const int input_index = (input_row * cols) + input_col;
          const float real_input_value = real.data[input_index];
          const float imaginary_input_value = imaginary.data[input_index];

          const float output_input_row =
              (output_row_float * input_row_float) / rows_float;
          const float output_input_col =
              output_col_float * input_col_float / cols_float;

          const float exponent =
              2.f * PI * (output_input_row + output_input_col);

          const float cos_exp = cos(exponent);
          const float sin_exp = sin(exponent);

          real_sum += ((real_input_value * cos_exp) -
                       (imaginary_input_value * sin_exp)) /
                      size_float;
          imaginary_sum += ((real_input_value * sin_exp) +
                            (imaginary_input_value * cos_exp)) /
                           size_float;
        }
      }

      const int output_index = (output_row * cols) + output_col;
      real_result.data[output_index] = real_sum;
      imaginary_result.data[output_index] = imaginary_sum;
    }
  }
}
#include "image_playground/file_helpers.h"
#include "image_playground/fourier.h"
#include "image_playground/gray_scale.h"
#include "image_playground/image.h"
#include "image_playground/math.h"
#include "image_playground/profile.h"

#include <cmath>
#include <stdio.h>
#include <stdlib.h>

static constexpr float kPi = 3.14159265359f;
static constexpr float kEpsilon = 0.01f;
const char *test_image_name = "assets/cln1.jpg";

// Recommended, but it takes a while.
static constexpr bool kCompareToGroundTruth = false;

int CompareImages(const FloatImage &test, const FloatImage &gt,
                  const float epsilon);

bool VerifyGroundTruth(const FloatImage &image, FloatImage &fourier_real,
                       FloatImage &fourier_imaginary);

void GTFourierTransform(const FloatImage &image, FloatImage &real,
                        FloatImage &imaginary);

void GTInverseFourierTransform(const FloatImage &real,
                               const FloatImage &imaginary,
                               FloatImage &real_result,
                               FloatImage &imaginary_result);

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

  const int image_size = image.rows * image.cols;

  FloatImage fourier_real(image.rows, image.cols);
  FloatImage fourier_imaginary(image.rows, image.cols);
  FourierTransform(gray, fourier_real, fourier_imaginary);

  const FloatImage visualization_image =
      VisualizationOutput(fourier_real, fourier_imaginary);
  WriteImageToPNG(visualization_image, output_directory / "fourier.png");

  const FloatImage inverse_fourier =
      InverseFourierTransform(fourier_real, fourier_imaginary);

  WriteImageToPNG(inverse_fourier, output_directory / "inverse_fourier.png");

  int error_count = CompareImages(inverse_fourier, gray, kEpsilon);
  printf("Errors in inverse transform: %d / %d\n", error_count, image_size);

  if (kCompareToGroundTruth) {
    FloatImage fourier_real_gt(image.rows, image.cols);
    FloatImage fourier_imaginary_gt(image.rows, image.cols);

    const bool valid_gt =
        VerifyGroundTruth(gray, fourier_real_gt, fourier_imaginary_gt);
    assert(valid_gt);

    const int real_fourier_errors =
        CompareImages(fourier_real, fourier_real_gt, kEpsilon);
    const int imaginary_fourier_errors =
        CompareImages(fourier_imaginary, fourier_imaginary_gt, kEpsilon);

    printf("Fourier errors. Real: %d / %d. Imaginary: %d / %d\n",
           real_fourier_errors, image_size, imaginary_fourier_errors,
           image_size);
  }

  return 0;
}

int CompareImages(const FloatImage &a, const FloatImage &b,
                  const float percent_epsilon) {
  assert(a.rows == b.rows && a.cols == b.cols);
  const int image_size = b.rows * b.cols;

  int error_count = 0;
  for (int i = 0; i < image_size; i++) {
    const float diff = AbsDiff(a.data[i], b.data[i]);
    const float percent_diff = diff / a.data[i];
    if (percent_diff > percent_epsilon) {
      const int row = (i / a.cols);
      const int col = (i % a.cols);
      printf("(%d, %d): %f -> %f (%f %%)\n", row, col, a.data[i], b.data[i],
             (percent_diff * 100.f));
      error_count++;
    }
  }

  return error_count;
}

bool VerifyGroundTruth(const FloatImage &image, FloatImage &fourier_real,
                       FloatImage &fourier_imaginary) {
  const int image_size = image.rows * image.cols;

  Profiler p;
  p.Start();
  GTFourierTransform(image, fourier_real, fourier_imaginary);
  p.EndAndPrintElapsedUS("GT Fourier Timing");

  FloatImage inverse_fourier_real(image.rows, image.cols);
  FloatImage inverse_fourier_imaginary(image.rows, image.cols);
  p.Start();
  GTInverseFourierTransform(fourier_real, fourier_imaginary,
                            inverse_fourier_real, inverse_fourier_imaginary);
  p.EndAndPrintElapsedUS("GT Inverse Fourier Timing");

  int error_count = CompareImages(inverse_fourier_real, image, kEpsilon);
  printf("GT Errors: %d / %d\n", error_count, image_size);
  return (error_count > 0);
}

void GTFourierTransform(const FloatImage &image, FloatImage &real,
                        FloatImage &imaginary) {
  assert(real.rows == image.rows && real.cols == image.cols);
  assert(imaginary.rows == image.rows && imaginary.cols == image.cols);

  constexpr float kNegTwoPi = -2.f * kPi;

  const float rows_float = static_cast<float>(image.rows);
  const float cols_float = static_cast<float>(image.cols);

  const float rows_inv = 1.f / rows_float;
  const float cols_inv = 1.f / cols_float;

  for (int output_row = 0; output_row < image.rows; output_row++) {
    printf("Ground truth fourier row: %d / %d\n", output_row, image.rows);
    const float output_row_float = static_cast<float>(output_row);
    for (int output_col = 0; output_col < image.cols; output_col++) {
      const float output_col_float = static_cast<float>(output_col);

      float real_sum = 0.f;
      float imaginary_sum = 0.f;
      int input_index = 0;

      for (int input_row = 0; input_row < image.rows; input_row++) {
        const float input_row_float = static_cast<float>(input_row);

        const float output_input_row =
            output_row_float * input_row_float * rows_inv;

        for (int input_col = 0; input_col < image.cols;
             input_col++, input_index++) {
          const float input_col_float = static_cast<float>(input_col);
          const float input_value = image.data[input_index];

          const float output_input_col =
              output_col_float * input_col_float * cols_inv;

          const float exponent =
              kNegTwoPi * (output_input_row + output_input_col);
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
    printf("Ground truth inverse fourier row: %d / %d\n", output_row, rows);
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
              2.f * kPi * (output_input_row + output_input_col);

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
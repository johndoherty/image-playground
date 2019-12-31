#include "image_playground/gray_scale.h"

FloatImage MakeGrayScaleImage(const RGBImage &image) {
  const int rows = image.rows;
  const int cols = image.cols;
  const int count = rows * cols;

  FloatImage result(rows, cols);

  for (int i = 0; i < count; i++) {
    const RGB &rgb = image.data[i];

    const float r = static_cast<float>(rgb.r) / 255.f;
    const float g = static_cast<float>(rgb.g) / 255.f;
    const float b = static_cast<float>(rgb.b) / 255.f;

    result.data[i] = (0.21 * r) + (0.72 * g) + (0.07 * b);
  }

  return result;
}
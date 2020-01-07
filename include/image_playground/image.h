#pragma once

#include "image_playground/color.h"

template <typename T> struct Image {
  static constexpr int kDataSize = sizeof(T);

  Image(int _rows, int _cols);
  ~Image();

  Image(Image &&other);
  Image &operator=(Image &&other);

  Image(const Image &image) = delete;
  Image &operator=(const Image &) = delete;

  T *data;
  int rows;
  int cols;
};

/* Types */
using ByteImage = Image<uint8_t>;
using FloatImage = Image<float>;
using RGBImage = Image<RGB>;
using RGBAImage = Image<RGBA>;

/* Helpers */
template <typename T> int ComputeImageSizeBytes(const Image<T> &image);
template <typename T> Image<T> CopyImage(const Image<T> &src);

#include "image_playground/templates/image.tpp"
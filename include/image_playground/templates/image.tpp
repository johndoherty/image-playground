#pragma once

#include "image_playground/image.h"

#include <cstring>

template <typename T>
Image<T>::Image(int _rows, int _cols) : rows(_rows), cols(_cols) {
  data = new T[ComputeImageSizeBytes(*this)];
}

template <typename T> Image<T>::~Image() { delete[] data; }

template <typename T>
Image<T>::Image(Image &&other)
    : data(other.data), rows(other.rows), cols(other.cols) {
  other.data = nullptr;
  other.rows = 0;
  other.cols = 0;
}

template <typename T> Image<T> &Image<T>::operator=(Image<T> &&other) {
  if (this != &other) {
    delete[] data;

    data = other.data;
    rows = other.rows;
    cols = other.cols;

    other.data = nullptr;
    other.rows = 0;
    other.cols = 0;
  }

  return *this;
}

/* Helpers */
template <typename T> int ComputeImageSizeBytes(const Image<T> &image) {
  return image.cols * image.rows * sizeof(T);
}

template <typename T>
int ComputeIndex(const Image<T> &image, int row, int col) {
  return (row * image.cols) + col;
}

template <typename T> Image<T> CopyImage(const Image<T> &src) {
  Image<T> dst(src.rows, src.cols);
  memcpy(dst.data, src.data, ComputeImageSizeBytes(src));
  return dst;
}

template <typename T> T &GetPixel(Image<T> &src, int row, int col) {
  return src.data[ComputeIndex(src, row, col)];
}

template <typename T>
const T &GetPixel(const Image<T> &image, int row, int col) {
  return image.data[ComputeIndex(image, row, col)];
}
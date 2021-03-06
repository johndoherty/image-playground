#include "image_playground/file_helpers.h"
#include "image_playground/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <cstring>

namespace internal {

template <typename T>
void WriteImageToPNG(const Image<T> &image, const std::string &filename) {
  constexpr int kDataSize = Image<T>::kDataSize;

  const int stride = image.cols * kDataSize;
  const int success = stbi_write_png(filename.c_str(), image.cols, image.rows,
                                     kDataSize, image.data, stride);

  if (!success) {
    printf("Error storing image to file: %s\n", filename.c_str());
  }
}

} // namespace internal

std::filesystem::path GetImagePlaygroundRoot() {
  std::filesystem::path current_file_path(__FILE__);
  return current_file_path.parent_path().parent_path();
}

std::optional<RGBImage> RGBImageFromFile(const std::string &filename) {
  int x = 0;
  int y = 0;
  int n = 0;

  uint8_t *data = stbi_load(filename.c_str(), &x, &y, &n, 3);

  if (!data) {
    printf("Unable to load image from file: %s\n", filename.c_str());
    return {};
  }

  RGBImage image(y, x);
  memcpy(image.data, data, ComputeImageSizeBytes(image));
  stbi_image_free(data);

  return image;
}

void WriteImageToPNG(const ByteImage &image, const std::string &filename) {
  internal::WriteImageToPNG(image, filename);
}

void WriteImageToPNG(const FloatImage &image, const std::string &filename) {
  const int elements = image.rows * image.cols;

  ByteImage byte_image(image.rows, image.cols);

  for (int i = 0; i < elements; i++) {
    const float clamped = std::max(std::min(image.data[i], 1.f), 0.f);
    byte_image.data[i] = static_cast<uint8_t>(clamped * 255.f);
  }

  internal::WriteImageToPNG(byte_image, filename);
}

void WriteImageToPNG(const RGBImage &image, const std::string &filename) {
  internal::WriteImageToPNG(image, filename);
}

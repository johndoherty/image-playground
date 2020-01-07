#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "image_playground/convolve.h"
#include "image_playground/image.h"

int main(int argc, char *argv[]) {
  constexpr int kImageDimension = 100;
  constexpr int kImageSize = kImageDimension * kImageDimension;
  constexpr int kKernelWidth = 3;
  constexpr int kKernelSize = kKernelWidth * kKernelWidth;

  FloatImage test_image(kImageDimension, kImageDimension);

  for (int i = 0; i < kImageSize; i++) {
    test_image.data[i] = static_cast<float>(i) / kImageSize;
  }

  float kernel[kKernelSize] = {0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f};
  const FloatImage identity = Convolve(test_image, kernel, kKernelWidth);

  int mismatch_count = 0;
  for (int i = 0; i < kImageSize; i++) {
    if (Abs(identity.data[i] - test_image.data[i]) > 0.001f) {
      printf("Mismatch: %d (Expected: %f, Result: %f)\n", i, test_image.data[i],
             identity.data[i]);
      mismatch_count++;
    }
  }

  printf("Mismatch count: %d / %d\n", mismatch_count, kImageSize);
  return mismatch_count;
}
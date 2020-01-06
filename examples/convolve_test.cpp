#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "image_playground/convolve.h"
#include "image_playground/image.h"

int main(int argc, char *argv[]) {
  FloatImage test_image(100, 100);
  const int test_image_size = test_image.rows * test_image.cols;

  for (int i = 0; i < test_image_size; i++) {
    test_image.data[i] = static_cast<float>(i) / test_image_size;
  }

  Kernel<3> kernel{{0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f}};
  const FloatImage identity = Convolve(test_image, kernel);

  int mismatch_count = 0;
  for (int i = 0; i < test_image_size; i++) {
    if (std::abs(identity.data[i] - test_image.data[i]) > 0.001f) {
      printf("Mismatch: %d (%f -> %f)\n", i, test_image.data[i],
             identity.data[i]);
      mismatch_count++;
    }
  }

  printf("Mismatch count: %d / %d\n", mismatch_count, test_image_size);
  return mismatch_count;
}
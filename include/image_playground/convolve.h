#pragma once

#include "image_playground/image.h"

FloatImage Convolve(const FloatImage &input, const float *const kernel,
                    const int kernel_width);
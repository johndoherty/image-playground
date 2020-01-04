#pragma once

#include "image_playground/image.h"

void FourierTransform(const FloatImage &image, FloatImage &real,
                      FloatImage &imaginary);
FloatImage InverseFourierTransform(const FloatImage &image);
FloatImage FourierTransformForVisualizing(const FloatImage &image);
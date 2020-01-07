#pragma once

#include "image_playground/image.h"

void FourierTransform(const FloatImage &image, FloatImage &real,
                      FloatImage &imaginary);

FloatImage InverseFourierTransform(const FloatImage &real,
                                   const FloatImage &imaginary);

FloatImage FourierTransformForVisualizing(const FloatImage &image);
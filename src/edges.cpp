#include "image_playground/edges.h"

#include <type_traits>

namespace {

template <int N>
void MakeKernel(float kernel[N * N]) {
    static_assert(N % 2 != 0, "Only odd sized kernes are allowed");

    int center = N / 2;
}

} // namespace

FloatImage MakeEdgeImage(const FloatImage& image) {
    FloatImage result = CopyImage(image);

    const int rows = image.rows;
    const int cols = image.cols;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {

        }
    }

    return result;
}

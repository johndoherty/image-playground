#include "image_playground/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <cstring>

namespace {
} // namespace

Image::Image(int _rows, int _cols, int _n)
    : rows(_rows), cols(_cols), n(_n) {
    data = new uint8_t[ComputeImageSize(rows, cols, n)];
}

Image::~Image() {
    delete[] data;
}

std::unique_ptr<Image> ImageFromFile(const std::string& filename) {
    int x = 0;
    int y = 0;
    int n = 0;

    uint8_t *data = stbi_load(filename.c_str(), &x, &y, &n, 0);
    std::unique_ptr<Image> image = std::make_unique<Image>(y, x, n);
    memcpy(image->data, data, ComputeImageSize(*image));
    stbi_image_free(data);

    return image;
}

std::unique_ptr<Image> CopyImage(const std::unique_ptr<Image>& src) {
    std::unique_ptr<Image> dst = std::make_unique<Image>(src->rows, src->cols, src->n);
    memcpy(dst->data, src->data, ComputeImageSize(*src));
    return dst;
}

void WriteImageToPNG(const std::unique_ptr<Image>& image, const std::string& filename) {
    const int stride = image->cols * image->n;
    const int success = stbi_write_png(filename.c_str(), image->cols, image->rows, image->n, image->data, stride);

    if (!success) {
        printf("Error storing image to file: %s\n", filename.c_str());
    }
}


int ComputeImageSize(int rows, int cols, int n) {
    return rows * cols * n;
}

int ComputeImageSize(const Image& image) {
    return ComputeImageSize(image.rows, image.cols, image.n);
}

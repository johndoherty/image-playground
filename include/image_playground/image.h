#pragma once

#include <string>
#include <memory>

template <typename T>
struct Image {
    static constexpr int kDataSize = sizeof(T);

    Image(int _rows, int _cols) : rows(_rows), cols(_cols) {
        data = new T[ComputeImageSize(*this)];
    }

    ~Image() {
        delete[] data;
    }

    Image(Image&& other)
        : data(other.data)
        , rows(other.rows)
        , cols(other.cols) {

        other.data = nullptr;
        other.rows = 0;
        other.cols = 0;
    }

    Image& operator=(Image&& other) {
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

    Image(const Image& image) = delete;
    Image& operator=(const Image&) = delete;

    T* data;
    int rows;
    int cols;
};

struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

using ByteImage = Image<uint8_t>;
using FloatImage = Image<float>;
using RGBImage = Image<RGB>;
using RGBAImage = Image<RGBA>;

RGBImage RGBImageFromFile(const std::string& filename);

void WriteImageToPNG(const ByteImage& image, const std::string& filename);
void WriteImageToPNG(const FloatImage& image, const std::string& filename);
void WriteImageToPNG(const RGBImage& image, const std::string& filename);

template <typename T>
Image<T> CopyImage(const Image<T>& src) {
    Image<T> dst(src.rows, src.cols);
    memcpy(dst.data, src.data, ComputeImageSize(src));
    return dst;
}

template <typename T>
int ComputeImageSize(const Image<T>& image) {
    return image.cols * image.rows * sizeof(T);
}

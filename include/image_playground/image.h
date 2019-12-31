#pragma once

#include <string>
#include <memory>

struct Image {
    Image(int rows, int cols, int n);
    ~Image();

    uint8_t* data;
    int rows;
    int cols;
    int n;
};

std::unique_ptr<Image> ImageFromFile(const std::string& filename);
std::unique_ptr<Image> CopyImage(const std::unique_ptr<Image>& src);

void WriteImageToPNG(const std::unique_ptr<Image>& image, const std::string& filename);

int ComputeImageSize(int rows, int cols, int n);
int ComputeImageSize(const Image& image);

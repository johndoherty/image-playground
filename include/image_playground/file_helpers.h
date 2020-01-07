#pragma once

#include <filesystem>
#include <optional>

#include "image_playground/image.h"

std::filesystem::path GetImagePlaygroundRoot();

std::optional<RGBImage> RGBImageFromFile(const std::string &filename);

void WriteImageToPNG(const ByteImage &image, const std::string &filename);
void WriteImageToPNG(const FloatImage &image, const std::string &filename);
void WriteImageToPNG(const RGBImage &image, const std::string &filename);
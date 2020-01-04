#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "image_playground/edges.h"
#include "image_playground/file_helpers.h"
#include "image_playground/fourier.h"
#include "image_playground/gray_scale.h"
#include "image_playground/image.h"

// TODO: Fourier output should be based on input file name
#if 0
const char *default_image = "assets/wales.jpg";
const char *default_fourier_image = "outputs/wales_fourier.jpg";
const char *default_output_image = "outputs/wales_edges.png";
const char *default_output_fourier_image = "outputs/wales_edges_fourier.png";
#else
const char *default_image = "assets/cln1.jpg";
const char *default_fourier_image = "outputs/cln_fourier.png";
const char *default_output_image = "outputs/cln_edges.png";
const char *default_output_fourier_image = "outputs/cln_edges_fourier.png";
#endif

int main(int argc, char *argv[]) {
  std::filesystem::path examples_directory =
      GetImagePlaygroundRoot() / "examples";

  std::filesystem::path input_file;
  if (argc < 2) {
    input_file = examples_directory / default_image;
  } else {
    input_file = std::filesystem::path(argv[1]);
  }

  std::filesystem::path fourier_file =
      examples_directory / default_fourier_image;

  std::filesystem::path output_fourier_file =
      examples_directory / default_output_fourier_image;

  std::filesystem::path output_file = examples_directory / default_output_image;
  std::optional<RGBImage> image_optional = RGBImageFromFile(input_file);

  if (!image_optional) {
    return 1;
  }

  const RGBImage image = std::move(*image_optional);
  const FloatImage gray_image = MakeGrayScaleImage(image);

  const FloatImage input_fourier = FourierTransformForVisualizing(gray_image);
  WriteImageToPNG(input_fourier, fourier_file);

  const FloatImage edge_image = MakeEdgeImage(gray_image);
  WriteImageToPNG(edge_image, output_file);

  const FloatImage output_fourier = FourierTransformForVisualizing(edge_image);
  WriteImageToPNG(output_fourier, output_fourier_file);

  return 0;
}
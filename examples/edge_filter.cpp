#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "image_playground/blur.h"
#include "image_playground/edges.h"
#include "image_playground/file_helpers.h"
#include "image_playground/fourier.h"
#include "image_playground/gray_scale.h"
#include "image_playground/image.h"

const char *default_assets = "assets";
const char *default_output = "outputs";

// const char *default_image_name = "wales.jpg";
const char *default_image_name = "cln1.jpg";

int main(int argc, char *argv[]) {
  using namespace std;
  using namespace filesystem;

  const path examples_directory = GetImagePlaygroundRoot() / "examples";
  const path default_output_directory = examples_directory / default_output;
  const path default_assets_directory = examples_directory / default_assets;

  // TODO: This could be specified by the user
  const path output_directory = default_output_directory;

  path input_file_path;
  if (argc < 2) {
    input_file_path = default_assets_directory / default_image_name;
  } else {
    input_file_path = path(argv[1]);
  }

  optional<RGBImage> image_optional = RGBImageFromFile(input_file_path);

  if (!image_optional) {
    return 1;
  }

  const string file_stem = input_file_path.stem();

  const RGBImage image = move(*image_optional);
  const FloatImage gray_image = MakeGrayScaleImage(image);

  const string fourier_file_name = file_stem + "_fourier.png";
  const FloatImage input_fourier = FourierTransformForVisualizing(gray_image);
  WriteImageToPNG(input_fourier, output_directory / fourier_file_name);

  const string blur_file_name = file_stem + "_blur.png";
  const FloatImage blur_image = MakeBlurImage(gray_image);
  WriteImageToPNG(blur_image, output_directory / blur_file_name);

  const string blur_fourier_file_name = file_stem + "_blur_fourier.png";
  const FloatImage blur_fourier = FourierTransformForVisualizing(blur_image);
  WriteImageToPNG(blur_fourier, output_directory / blur_fourier_file_name);

  const string edge_file_name = file_stem + "_edges.png";
  const FloatImage edge_image = MakeEdgeImage(gray_image);
  WriteImageToPNG(edge_image, output_directory / edge_file_name);

  const string edge_fourier_file_name = file_stem + "_edges_fourier.png";
  const FloatImage edge_fourier = FourierTransformForVisualizing(edge_image);
  WriteImageToPNG(edge_fourier, output_directory / edge_fourier_file_name);

  return 0;
}
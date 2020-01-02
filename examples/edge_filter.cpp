#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "image_playground/edges.h"
#include "image_playground/gray_scale.h"
#include "image_playground/image.h"
#include "image_playground/file_helpers.h"

const char *default_image = "assets/wales.jpg";
const char *default_output_image = "outputs/wales_edges.png";

int main(int argc, char *argv[])
{
  std::filesystem::path examples_directory = GetImagePlaygroundRoot() / "examples";

  std::filesystem::path input_file;
  if (argc < 2)
  {
    input_file = examples_directory / default_image;
  }
  else
  {
    input_file = std::filesystem::path(argv[1]);
  }

  std::filesystem::path output_file = examples_directory / default_output_image;
  std::optional<RGBImage> image_optional = RGBImageFromFile(input_file);

  if (!image_optional)
  {
    return 1;
  }

  const RGBImage image = std::move(*image_optional);
  const FloatImage gray_image = MakeGrayScaleImage(image);

  const Image edge_image = MakeEdgeImage(gray_image);
  WriteImageToPNG(edge_image, output_file);

  return 0;
}
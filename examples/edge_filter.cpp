#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "image_playground/edges.h"
#include "image_playground/gray_scale.h"
#include "image_playground/image.h"

// TODO: Use <filesystem> to get absolute paths
// Right now this needs to be run from the repo's base directory
const char *default_image = "assets/wales.jpg";
const char *default_output_image = "outputs/wales_edges.png";

int main(int argc, char *argv[]) {
  const char *image_file = default_image;
  if (argc > 2) {
    image_file = argv[1];
  }

  const std::string filename(image_file);
  std::optional<RGBImage> image_optional = RGBImageFromFile(filename);

  if (!image_optional) {
    return 1;
  }

  const RGBImage image = std::move(*image_optional);
  const FloatImage gray_image = MakeGrayScaleImage(image);

  const Image edge_image = MakeEdgeImage(gray_image);
  const std::string output_filename(default_output_image);
  WriteImageToPNG(edge_image, output_filename);

  return 0;
}
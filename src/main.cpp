#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "image_playground/edges.h"
#include "image_playground/gray_scale.h"
#include "image_playground/image.h"

const char *default_image = "/Users/john/Desktop/P8023034.jpg";
const char *default_output_image = "output.png";

int main(int argc, char *argv[]) {
  const char *image_file = default_image;
  if (argc > 2) {
    image_file = argv[1];
  }

  const std::string filename(image_file);
  const RGBImage image = RGBImageFromFile(filename);
  const FloatImage gray_image = MakeGrayScaleImage(image);

  const Image edge_image = MakeEdgeImage(gray_image);
  const std::string output_filename(default_output_image);
  WriteImageToPNG(edge_image, output_filename);

  return 0;
}
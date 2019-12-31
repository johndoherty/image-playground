#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "image_playground/image.h"
#include "image_playground/edges.h"

const char * default_image = "/Users/john/Desktop/P8023034.jpg";
const char * default_output_image = "output.png";

int main(int argc, char* argv[]) {
    const char* image_file = default_image;
    if (argc > 2) {
        image_file = argv[1];
    }

    const std::string filename(image_file);
    const std::unique_ptr<Image> image = ImageFromFile(filename);
    printf("Rows: %d, Cols: %d, N: %d, data: %p\n",
        image->rows, image->cols, image->n, image->data);

    const std::unique_ptr<Image> edge_image = MakeEdgeImage(image);
    const std::string output_filename(default_output_image);
    WriteImageToPNG(edge_image, output_filename);

    return 0;
}
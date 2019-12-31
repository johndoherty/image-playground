#include "image_playground/edges.h"

std::unique_ptr<Image> MakeEdgeImage(const std::unique_ptr<Image>& image) {
    std::unique_ptr<Image> result = CopyImage(image);
    return result;
}

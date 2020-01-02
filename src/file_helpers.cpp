#include "image_playground/file_helpers.h"

std::filesystem::path GetImagePlaygroundRoot()
{
    std::filesystem::path current_file_path(__FILE__);
    return current_file_path.parent_path().parent_path();
}
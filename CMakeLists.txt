cmake_minimum_required(VERSION 3.1)

project(ImagePlayground VERSION 1.0 DESCRIPTION "Basic image filtering." LANGUAGES CXX)

# C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external)

set(SOURCE
    src/blur.cpp
    src/color.cpp
    src/convolve.cpp
    src/edges.cpp
    src/file_helpers.cpp
    src/fourier.cpp
    src/gray_scale.cpp
    src/profile.cpp)

add_library(image_playground ${SOURCE})
target_include_directories(image_playground PUBLIC include)

add_subdirectory(examples)
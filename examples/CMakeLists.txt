set(SOURCE src/main.cpp src/image.cpp src/edges.cpp src/gray_scale.cpp)

add_executable(edge_filter edge_filter.cpp)
target_link_libraries(edge_filter image_playground)

add_executable(convolve_test convolve_test.cpp)
target_link_libraries(convolve_test image_playground)

add_executable(fourier_test fourier_test.cpp)
target_link_libraries(fourier_test image_playground)
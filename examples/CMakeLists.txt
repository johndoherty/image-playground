set(SOURCE src/main.cpp src/image.cpp src/edges.cpp src/gray_scale.cpp)

add_executable(edge_filter edge_filter.cpp)
target_link_libraries(edge_filter image_playground)
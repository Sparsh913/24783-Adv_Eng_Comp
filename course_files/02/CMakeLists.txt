cmake_minimum_required(VERSION 3.8)
project(bouncing_ball)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(3dmaze)
add_subdirectory(floodfill)
add_subdirectory(shoot)
add_subdirectory(bouncing_ball)
add_subdirectory(drawing_tool)
add_subdirectory(../public/src public)

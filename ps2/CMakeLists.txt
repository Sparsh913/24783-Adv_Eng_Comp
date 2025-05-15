cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0079 NEW)
project(ps2)

# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(bubblesort)
add_subdirectory(delaunay)
# add_subdirectory(../../MMLPlayer ${CMAKE_BINARY_DIR}/mmlplayer)
add_subdirectory(../../public/src ${CMAKE_BINARY_DIR}/public)

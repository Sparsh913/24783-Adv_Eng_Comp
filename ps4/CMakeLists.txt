cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0079 NEW)
project(ps4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ps4_1)
add_subdirectory(../../public/src ${CMAKE_BINARY_DIR}/public)
add_subdirectory(../../MMLPlayer/ym2612 ${CMAKE_BINARY_DIR}/ym2612)
add_subdirectory(../../solnp solnp)
include_directories(../../solnp/src)
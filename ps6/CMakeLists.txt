cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0079 NEW)
project(ps6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ps6_1)
add_subdirectory(ps6_2)
add_subdirectory(ps6_3)
add_subdirectory(bintreelib)
add_subdirectory(../../public/src ${CMAKE_BINARY_DIR}/public)

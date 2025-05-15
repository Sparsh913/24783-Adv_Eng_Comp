cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0079 NEW)
project(ps7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(../../public/src ${CMAKE_BINARY_DIR}/public)

add_executable(ps9 MACOSX_BUNDLE ps9.cpp)
target_link_libraries(ps9 fssimplewindow ysclass ysgl)

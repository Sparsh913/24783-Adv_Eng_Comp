include_directories(${CMAKE_SOURCE_DIR}/bitmap)
include_directories(${CMAKE_SOURCE_DIR}/hash)
add_executable(ps5_2 MACOSX_BUNDLE main.cpp)
target_link_libraries(ps5_2 bitmap fssimplewindow ysbitmapfont ysclass)
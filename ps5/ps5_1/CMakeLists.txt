include_directories(${CMAKE_SOURCE_DIR}/bitmap)
# add_library(bitmap simplebitmap.h simplebitmap.cpp simplebitmaptemplate.h)
add_executable(ps5_1 MACOSX_BUNDLE main.cpp)
target_link_libraries(ps5_1 bitmap fssimplewindow ysbitmapfont ysclass)
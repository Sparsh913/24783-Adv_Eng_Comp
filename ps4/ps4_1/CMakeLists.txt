add_library(data point.h ffd.h data.h data.cpp)
add_executable(ps4_1 MACOSX_BUNDLE ps4_1.cpp)
include_directories(${CMAKE_SOURCE_DIR}/../../solnp/src)
target_link_libraries(ps4_1 data dlib fssimplewindow ysbitmapfont ysclass) 
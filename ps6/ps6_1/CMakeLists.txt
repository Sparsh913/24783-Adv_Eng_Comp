include_directories(${CMAKE_SOURCE_DIR}/bintreelib)
add_executable(ps6_1 MACOSX_BUNDLE main.cpp)
target_link_libraries(ps6_1 bintreelib fssimplewindow ysbitmapfont ysclass)

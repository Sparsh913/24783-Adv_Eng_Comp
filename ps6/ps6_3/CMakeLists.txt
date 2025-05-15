include_directories(${CMAKE_SOURCE_DIR}/bintreelib)
add_executable(ps6_3 MACOSX_BUNDLE main.cpp)
target_link_libraries(ps6_3 bintreelib fssimplewindow ysbitmapfont ysclass)

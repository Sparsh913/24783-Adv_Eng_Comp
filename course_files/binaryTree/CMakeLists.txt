add_subdirectory(../public public)
add_executable(bintreevis MACOSX_BUNDLE bintreevis.cpp)
target_link_libraries(bintreevis fssimplewindow ysbitmapfont)

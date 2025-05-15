add_library(tile_map tiles.h tiles.cpp map.h map.cpp)
add_executable(ps3_1 MACOSX_BUNDLE ps3_1.cpp)
target_link_libraries(ps3_1 tile_map fssimplewindow ysbitmapfont ysclass)
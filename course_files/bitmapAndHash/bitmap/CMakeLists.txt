include_directories(.)

add_library(simplebitmap simplebitmaptemplate.h simplebitmap.cpp simplebitmap.h)

add_library(png yspng.h yspng.cpp)

add_executable(pngviewer pngviewer.cpp)
target_link_libraries(pngviewer simplebitmap png fssimplewindow)

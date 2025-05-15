cmake_minimum_required(VERSION 3.10)

project(DigiClock)

# Add the executable target
add_executable(digiclock MACOSX_BUNDLE main.cpp)

# Link the required libraries
target_link_libraries(digiclock simplewindow bitmapfont)

add_library(fgets Fgets.h Fgets.cpp)

add_library(parser parserClass.h parserClass.cpp)

add_executable(main main.cpp)
target_link_libraries(main parser fgets)

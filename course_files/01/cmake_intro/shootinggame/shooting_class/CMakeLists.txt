add_library(shooting_game 
shoot-3-class.cpp
shoot-3-class.h)
target_include_directories(shooting_game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(shooting_game simplewindow ysglfontdata)

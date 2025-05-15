add_subdirectory(../public public)

add_subdirectory(../solnp solnp)
include_directories(../solnp/src)

add_executable(toy1 toy1.cpp)
target_link_libraries(toy1 dlib)

add_executable(toy2 toy2.cpp)
target_link_libraries(toy2 dlib fssimplewindow)

add_executable(notsotoy notsotoy.cpp)
target_link_libraries(notsotoy dlib fssimplewindow)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Determine the platform and set source files accordingly
if("${CMAKE_SYSTEM_NAME}" STREQUAL "WindowsStore")
    message(FATAL_ERROR "This framework is not supported for UWP")
elseif(MSVC)
    set(LIB_SRCS
        windows/fssimplewindow.cpp
        windows/fssimplewindow.h
    )
    set(HEADER_PATH windows)
elseif(APPLE)
    set(LIB_SRCS
        macosx/fssimplewindow.h
        macosx/fssimplewindowcpp.cpp
        macosx/fssimplewindowobjc.m
    )
    set(HEADER_PATH macosx)
elseif(UNIX)
    set(LIB_SRCS
        linux/fssimplewindow.cpp
        linux/fssimplewindow.h
    )
    set(HEADER_PATH linux)

    # Add Unix-specific libraries
    find_package(X11 REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(Threads REQUIRED)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Add the simplewindow library
add_library(simplewindow ${LIB_SRCS})
target_include_directories(simplewindow PUBLIC ${HEADER_PATH})

# Link system libraries (only for Unix)
if(UNIX)
    target_link_libraries(simplewindow PUBLIC
        ${X11_LIBRARIES}       # X11 libraries
        OpenGL::GL             # OpenGL library
        OpenGL::GLU            # OpenGL utility library
        Threads::Threads       # pthread library
    )
endif()

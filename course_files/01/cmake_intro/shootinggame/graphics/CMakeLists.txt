set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if("${CMAKE_SYSTEM_NAME}" STREQUAL "WindowsStore")
    message(FATAL_ERROR "This framework not supported for UWP")
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
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

add_library(simplewindow ${LIB_SRCS})
target_include_directories(simplewindow PUBLIC ${HEADER_PATH})

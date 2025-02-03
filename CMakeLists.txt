cmake_minimum_required(VERSION 3.23)

project(u-bahn VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable this to debug cmake scripts:
set(CMAKE_VERBOSE_MAKEFILE OFF)

if (CMAKE_SYSTEM_NAME STREQUAL Android)
    set(ANDROID_NDK_ROOT "$ENV{ANDROID_NDK_ROOT}" CACHE PATH "Instalation dir of the Android NDK")
    message("Using android NDK: ${ANDROID_NDK_ROOT}")
endif ()

enable_testing()

add_subdirectory(src)

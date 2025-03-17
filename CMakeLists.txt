# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.23)

project(u-bahn VERSION 0.1 LANGUAGES CXX)

set(UBAHN_VERSION "dev" CACHE STRING "The app version to show to the user")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable this to debug cmake scripts:
set(CMAKE_VERBOSE_MAKEFILE OFF)

message(STATUS "U-Bahn app version: ${UBAHN_VERSION}")

if (CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL Android)
    set(ANDROID_NDK_ROOT "$ENV{ANDROID_NDK_ROOT}" CACHE PATH "Instalation dir of the Android NDK")
    message(STATUS "Using android NDK: ${ANDROID_NDK_ROOT}")
endif ()

enable_testing()

add_subdirectory(proto)
add_subdirectory(src)

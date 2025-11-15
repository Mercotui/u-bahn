# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT

find_package(SDL3 REQUIRED)
find_package(absl REQUIRED)

add_library(platform STATIC platform.cpp)
add_library(Platform::Platform ALIAS platform)

target_link_libraries(platform PUBLIC SDL3::SDL3 GLAD::GLAD abseil::abseil)

# We must hide the headers behind the target include dir, so that they are only accessible if you link the target.
# This ensures that the platform compile definitions are also propagated, as these change the behaviour of the headers.
target_include_directories(platform PUBLIC include)

if (CMAKE_SYSTEM_NAME STREQUAL Android)
    message(NOTICE "Platform lib using PLATFORM_ANDROID")
    target_compile_definitions(platform PUBLIC PLATFORM_ANDROID)
elseif (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    message(NOTICE "Platform lib using PLATFORM_WEB")
    target_compile_definitions(platform PUBLIC PLATFORM_WEB)
else ()
    message(NOTICE "Platform lib using default PLATFORM_DESKTOP")
    target_compile_definitions(platform PUBLIC PLATFORM_DESKTOP)
endif ()

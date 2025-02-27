# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT
add_library(platform STATIC platform.cpp)
add_library(Platform::Platform ALIAS platform)

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

# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT

find_package(SDL3 REQUIRED)
find_package(absl REQUIRED)
find_package(GTest REQUIRED)

add_library(platform STATIC
        private/clock.cpp
        private/input_handlers.cpp
        private/input_helpers.cpp
        private/input_manager.cpp
        private/keyboard_mouse.cpp
        private/platform.cpp
)
add_library(Platform::Platform ALIAS platform)

target_compile_definitions(platform PUBLIC
        $<$<PLATFORM_ID:Android>:PLATFORM_ANDROID>
        $<$<PLATFORM_ID:Emscripten>:PLATFORM_WEB>
)

target_link_libraries(platform PUBLIC SDL3::SDL3 GLAD::GLAD abseil::abseil)

# We must hide the headers behind the target include dir, so that they are only accessible if you link the target.
# This ensures that the platform compile definitions are also propagated, as these change the behaviour of the headers.
target_include_directories(platform PUBLIC include)

add_executable(platform_test
        private/input_helpers_unittest.cpp
)
target_link_libraries(platform_test PRIVATE
        GTest::gtest GTest::gtest_main
        Platform::Platform
)
add_test(NAME platform_test COMMAND platform_test)

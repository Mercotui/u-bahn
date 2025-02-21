# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT

find_package(GTest REQUIRED)
find_package(absl REQUIRED COMPONENTS Log)
find_package(raylib 5.0 REQUIRED)

add_library(camera STATIC private/camera.cpp)
add_library(Game::Camera ALIAS camera)

target_link_libraries(camera raylib Game::World)

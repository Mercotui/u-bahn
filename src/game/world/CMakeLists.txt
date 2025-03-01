# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT

find_package(GTest REQUIRED)
find_package(absl REQUIRED COMPONENTS Log)
find_package(raylib 5.0 REQUIRED)
find_package(glm REQUIRED)
find_package(bezier REQUIRED)
find_package(mp-units REQUIRED)
find_package(benchmark REQUIRED)

add_library(game_world STATIC
        private/rail_segment.cpp
        private/rails.cpp
        private/train.cpp
        private/world.cpp
)
add_library(Game::World ALIAS game_world)
target_link_libraries(game_world PRIVATE
        raylib glm::glm absl::log bezier::bezier
        platform
)
target_link_libraries(game_world PUBLIC mp-units::mp-units)

add_executable(world_test
        private/rails_unittest.cpp
)
target_link_libraries(world_test PRIVATE raylib game_world GTest::gtest GTest::gtest_main)
add_test(NAME world_test COMMAND world_test)

add_executable(world_benchmark
        private/rails_benchmark.cpp)
target_link_libraries(world_benchmark raylib game_world benchmark::benchmark)

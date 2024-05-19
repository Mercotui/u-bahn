find_package(GTest REQUIRED)
find_package(absl REQUIRED COMPONENTS Log)
find_package(raylib 5.0 REQUIRED)
find_package(glm REQUIRED)
find_package(bezier REQUIRED)

add_library(game_world STATIC
        private/rail_segment.cpp
        private/rails.cpp
        private/train.cpp
)
add_library(Game::World ALIAS game_world)
target_link_libraries(game_world PRIVATE raylib glm::glm absl::log bezier::bezier)

add_executable(world_test
        private/train_unittest.cpp
        private/rails_unittest.cpp
)
target_link_libraries(world_test PRIVATE raylib game_world GTest::gtest GTest::gtest_main)
add_test(NAME world_test COMMAND world_test)

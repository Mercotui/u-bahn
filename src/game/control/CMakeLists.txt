find_package(GTest REQUIRED)
find_package(absl REQUIRED COMPONENTS Log)

add_library(game_control STATIC
        private/control_scheme_mapper.cpp
)
add_library(Game::Control ALIAS game_control)
target_link_libraries(game_control PRIVATE absl::log)

add_executable(control_test
        private/control_scheme_mapper_unittest.cpp
)
target_link_libraries(control_test PRIVATE game_control GTest::gtest GTest::gtest_main)
add_test(NAME control_test COMMAND control_test)

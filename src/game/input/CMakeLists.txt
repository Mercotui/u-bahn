find_package(GTest REQUIRED)
find_package(absl REQUIRED COMPONENTS Log)
add_library(game_input STATIC
        private/input_manager.cpp
        private/joystick_handler.cpp
        private/raylib_input_handlers.cpp
)
add_library(Game::Input ALIAS game_input)
target_link_libraries(game_input PRIVATE absl::log enjoy platform)

add_executable(input_test
        private/input_manager_unittest.cpp
        private/joystick_handler_unittest.cpp
        private/raylib_input_handlers_unittest.cpp
)
target_link_libraries(input_test PRIVATE game_input platform enjoy GTest::gtest GTest::gtest_main)
add_test(NAME input_test COMMAND input_test)

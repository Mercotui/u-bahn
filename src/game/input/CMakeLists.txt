find_package(GTest REQUIRED)
find_package(absl REQUIRED COMPONENTS Log)
find_package(raylib 5.0 REQUIRED)

if (TARGET enjoy)
    message(NOTICE "Building game input joystick support")
    add_library(game_input_joystick STATIC
            private/joystick_handler.cpp
    )
    target_link_libraries(game_input_joystick PRIVATE absl::log enjoy)
    target_compile_definitions(game_input_joystick PUBLIC HAS_LIB_GAME_INPUT_JOYSTICK)
else ()
    message(NOTICE "NOT building game input joystick support")
endif ()

add_library(game_input STATIC
        private/input_axis_helpers.cpp
        private/input_manager.cpp
        private/raylib_input_handlers.cpp
        private/raylib_input_helpers.cpp
        private/keyboard_mouse.cpp
)
add_library(Game::Input ALIAS game_input)
target_link_libraries(game_input PRIVATE absl::log raylib $<TARGET_NAME_IF_EXISTS:game_input_joystick> platform)

add_executable(input_test
        private/input_manager_unittest.cpp
        private/joystick_handler_unittest.cpp
        private/raylib_input_handlers_unittest.cpp
        private/input_axis_helpers_unittest.cpp
)
target_link_libraries(input_test PRIVATE game_input game_input_joystick platform GTest::gtest GTest::gtest_main)
add_test(NAME input_test COMMAND input_test)

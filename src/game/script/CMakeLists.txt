find_package(sol2 REQUIRED)
find_package(absl REQUIRED COMPONENTS Log)
find_package(mp-units REQUIRED)
find_package(raylib 5.0 REQUIRED)

add_library(game_script private/script_interpreter.cpp private/script_binding.cpp)
add_library(Game::Script ALIAS game_script)

target_link_libraries(game_script PUBLIC Game::World sol2::sol2 absl::log raylib mp-units::mp-units)

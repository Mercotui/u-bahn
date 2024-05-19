find_package(raylib 5.0 REQUIRED)
find_package(absl REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(platform)
add_subdirectory(game/input)
add_subdirectory(game/control)
add_subdirectory(game/world)

add_executable(u-bahn
        main.cpp
        game/game.cpp
)

target_link_libraries(u-bahn PRIVATE raylib abseil::abseil platform Game::Input Game::Control Game::World)

include(GNUInstallDirs)
install(TARGETS u-bahn
        BUNDLE DESTINATION u-bahn
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

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

if (CMAKE_SYSTEM_NAME STREQUAL Android)
    message(WARNING "Android not yet implemented")
elseif (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    target_link_options(u-bahn PRIVATE
            -sUSE_GLFW=3
            -sASSERTIONS=1
            -sWASM=1
            -sASYNCIFY=1
            # TODO(Menno 02.06.2024) This option is introduced emsdk 3.1.51
            # -sGL_ENABLE_GET_PROC_ADDRESS=1
            --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/platform/emscripten/shell.html
            "--preload-file /home/menno/Workspace/u-bahn/resources/lowpoly_berlin_u-bahn/untitled.glb\
            @/home/menno/Workspace/u-bahn/resources/lowpoly_berlin_u-bahn/untitled.glb"
    )
    set_target_properties(u-bahn PROPERTIES SUFFIX ".html")
else ()
    add_subdirectory(third_party/libenjoy)
endif ()

target_link_libraries(u-bahn PRIVATE raylib abseil::abseil platform Game::Input Game::Control Game::World)

include(GNUInstallDirs)
install(TARGETS u-bahn
        BUNDLE DESTINATION u-bahn
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

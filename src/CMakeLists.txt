find_package(raylib 5.0 REQUIRED)
find_package(absl REQUIRED)

include(cmake/resources.cmake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_SYSTEM_NAME STREQUAL Android)
    add_library(u-bahn SHARED)
else ()
    add_executable(u-bahn)
endif ()

target_sources(u-bahn PRIVATE
        main.cpp
        game/game.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL Android)
    target_sources(u-bahn PRIVATE ${ANDROID_NDK_ROOT}/sources/android/native_app_glue/android_native_app_glue.c)
    target_link_options(u-bahn PUBLIC -u ANativeActivity_onCreate)

    target_link_libraries(u-bahn PRIVATE log android EGL GLESv2)
elseif (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    target_link_options(u-bahn PRIVATE
            -sUSE_GLFW=3
            -sASSERTIONS=1
            -sWASM=1
            -sASYNCIFY=1
            # TODO(Menno 02.06.2024) This option is introduced emsdk 3.1.51
            # -sGL_ENABLE_GET_PROC_ADDRESS=1
            --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/platform/emscripten/shell.html
    )
    set_target_properties(u-bahn PROPERTIES SUFFIX ".html")
    # Cmake doesn't detect the files emcc creates, install them explicitly to the root of the install prefix
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/u-bahn.html DESTINATION . RENAME index.html)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/u-bahn.js DESTINATION .)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/u-bahn.wasm DESTINATION .)
else ()
    add_subdirectory(third_party/libenjoy)
    include(GNUInstallDirs)
    install(TARGETS u-bahn
            BUNDLE DESTINATION u-bahn
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif ()

add_subdirectory(utils)
add_subdirectory(platform)
add_subdirectory(game/input)
add_subdirectory(game/control)
add_subdirectory(game/world)
add_subdirectory(game/camera)

target_link_libraries(u-bahn PRIVATE raylib
        abseil::abseil
        Version::Version platform Game::Input Game::Control Game::World Game::Camera)
target_add_resources(u-bahn ${CMAKE_SOURCE_DIR}/resources/ resources/)

include(cmake/packaging.cmake)

find_package(raylib 5.0 REQUIRED)

add_executable(u-bahn
        main.cpp
        game/game.cpp

)

target_include_directories(u-bahn PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(u-bahn PRIVATE raylib)

include(GNUInstallDirs)
install(TARGETS u-bahn
        BUNDLE DESTINATION u-bahn
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

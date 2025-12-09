# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT

find_package(GTest REQUIRED)
find_package(absl REQUIRED COMPONENTS Log)
find_package(fastgltf REQUIRED)
find_package(glm REQUIRED)

add_library(rendering STATIC
        private/camera.cpp
        private/model_data_packer.cpp
        private/model_cache.cpp
)
add_library(Game::Rendering ALIAS rendering)

target_link_libraries(rendering PUBLIC Utils::FileReader glm::glm)
target_link_libraries(rendering PRIVATE
        Platform::Platform Game::World
        abseil::abseil fastgltf::fastgltf fastgltf::stream_reader
)

add_executable(rendering_test
        private/gl_mock.cpp
        private/model_cache_unittest.cpp
        private/model_data_packer_unittest.cpp
)
target_link_libraries(rendering_test PRIVATE
        Game::Rendering
        Platform::Platform GLAD::GLAD abseil::abseil fastgltf::fastgltf GTest::gtest GTest::gtest_main GTest::gmock
)
add_test(NAME rendering_test COMMAND rendering_test)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/private/test_cube.glb"
        "${CMAKE_CURRENT_BINARY_DIR}/test_cube.glb" COPYONLY)

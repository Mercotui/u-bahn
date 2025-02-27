# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT
find_package(absl REQUIRED COMPONENTS Log)
find_package(watcher REQUIRED)

add_library(utils_file_reader STATIC
        private/file_reader.cpp
)
add_library(Utils::FileReader ALIAS utils_file_reader)
target_link_libraries(utils_file_reader PRIVATE
        absl::log watcher::watcher
        Platform::Platform
)

add_library(utils_version STATIC private/version.cpp)
add_library(Utils::Version ALIAS utils_version)
target_compile_definitions(utils_version PRIVATE UBAHN_VERSION="${UBAHN_VERSION}")

# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT
add_library(version STATIC private/version.cpp)
add_library(Version::Version ALIAS version)

target_compile_definitions(version PRIVATE UBAHN_VERSION="${UBAHN_VERSION}")

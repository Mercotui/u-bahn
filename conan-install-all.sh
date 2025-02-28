#!/usr/bin/env sh
#
# SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
# SPDX-License-Identifier: MIT
#

conan install -u -pr=profiles/linux-wayland.profile --settings=build_type=Debug --build=missing . &&
conan install -u -pr:b=default -pr:h=profiles/android.profile --settings=build_type=Release --build=missing . &&
conan install -u -pr:b=default -pr:h=profiles/emscripten.profile --build=missing  --settings=build_type=Debug .

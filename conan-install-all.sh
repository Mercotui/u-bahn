#!/usr/bin/env sh

conan install -u -pr=profiles/linux-wayland.profile --settings=build_type=Debug --build=missing . &&
conan install -u -pr:b=default -pr:h=profiles/android.profile --settings=build_type=Release --build=missing . &&
conan install -u -pr:b=default -pr:h=profiles/emscripten.profile --build=missing  --settings=build_type=Release .

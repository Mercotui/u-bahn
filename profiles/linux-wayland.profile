[settings]
os=Linux
arch=x86_64
compiler=gcc
compiler.version=14
compiler.cppstd=gnu26
compiler.libcxx=libstdc++11
build_type=Release

[options]
glfw/*:with_x11=False
glfw/*:with_wayland=True
xkbcommon/*:with_x11=False
xkbcommon/*:with_wayland=True

[conf]
# This line will make sure that the cmake_layout creates a separate build folders for our linux target
tools.cmake.cmake_layout:build_folder_vars=['settings.os']

[settings]
os=Linux
arch=x86_64
compiler=gcc
compiler.version=15
compiler.cppstd=gnu26
compiler.libcxx=libstdc++11
build_type=Release

[conf]
# This line will make sure that the cmake_layout creates a separate build folders for our linux target
tools.cmake.cmake_layout:build_folder_vars=['settings.os']
tools.cmake.cmaketoolchain:generator=Ninja

[platform_requires]
# dnf install cmake SDL3-devel
sdl/3.2.24
cmake/3.31.6

[platform_tool_requires]
cmake/3.31.6

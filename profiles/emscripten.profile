[settings]
os=Emscripten
arch=wasm
compiler=clang
compiler.version=18
compiler.libcxx=libc++
compiler.cppstd=23
build_type=Release

[options]
mp-units/*:std_format=True
mp-units/*:import_std=False

[tool_requires]
emsdk/3.1.73

[conf]
# This line will make sure that the cmake_layout creates a separate build folder for our emscripten target
tools.cmake.cmake_layout:build_folder_vars=['settings.os']
tools.cmake.cmaketoolchain:generator=Ninja

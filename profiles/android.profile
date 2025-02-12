[settings]
os=Android
# Pick your own desired API level
os.api_level=21
# Check which ARM architecture your phone has
arch=armv8
compiler=clang
compiler.libcxx=c++_static
# Check the compiler and cppstd versions if you change the android-ndk package version
compiler.version=18
compiler.cppstd=23

[tool_requires]
android-ndk/r27

[conf]
# This line will make sure that the cmake_layout creates a separate build folder for our emscripten target
tools.cmake.cmake_layout:build_folder_vars=['settings.os']

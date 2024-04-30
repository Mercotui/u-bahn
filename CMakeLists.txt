cmake_minimum_required(VERSION 3.23)

project(u-bahn VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/third_party/libenjoy)
add_subdirectory(src)

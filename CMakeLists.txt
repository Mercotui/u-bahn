cmake_minimum_required(VERSION 3.23)

project(u-bahn VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable this to debug cmake scripts:
set(CMAKE_VERBOSE_MAKEFILE OFF)

enable_testing()

add_subdirectory(src)

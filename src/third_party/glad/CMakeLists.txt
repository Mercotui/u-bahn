add_library(glad STATIC src/glad.cpp)
target_include_directories(glad PUBLIC include)
add_library(GLAD::GLAD ALIAS glad)

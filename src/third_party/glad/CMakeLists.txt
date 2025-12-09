add_library(glad STATIC src/gles2.c)
set_source_files_properties(src/gles2.c PROPERTIES LANGUAGE CXX)
target_include_directories(glad PUBLIC include)
add_library(GLAD::GLAD ALIAS glad)

find_package(fastgltf REQUIRED)

add_library(fastgltf_stream_reader STATIC src/stream_reader.cpp)
add_library(fastgltf::stream_reader ALIAS fastgltf_stream_reader)

target_include_directories(fastgltf_stream_reader PUBLIC include)
target_link_libraries(fastgltf_stream_reader PRIVATE fastgltf::fastgltf)

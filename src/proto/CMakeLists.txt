find_package(Protobuf REQUIRED)

add_library(buffers_world STATIC mercotui/ubahn/world/v1/world.proto)
protobuf_generate(TARGET buffers_world)
target_link_libraries(buffers_world PUBLIC protobuf::protobuf)
# TODO(Menno 26.02.2025) It is unfortunate to have to pollute the include directories with the current binary dir,
# but it seems there is no easy way to change the protoc output dir without a custom protoc wrapper.
target_include_directories(buffers_world PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
add_library(Buffers::World ALIAS buffers_world)

// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/rendering/private/mesh_packer.h"

#include <absl/log/log.h>

#include "platform/gl.h"

ModelData MeshPacker::PackBuffers(const fastgltf::Asset& asset) {
  ModelData data;

  glGenVertexArrays(1, &data.vertex_array_object);
  glGenBuffers(1, &data.vertex_buffer_object);

  glBindVertexArray(data.vertex_array_object);
  glBindBuffer(GL_ARRAY_BUFFER, data.vertex_buffer_object);

  if (asset.buffers.size() != 1) {
    // TODO(Menno 25.12.2025) This is a very small subset of GLTF, but it should be fine for now.
    LOG(FATAL) << "Expected exactly one data buffer in GLTF file, but got " << asset.buffers.size();
  }

  const fastgltf::Buffer& buffer = asset.buffers[0];
  auto& bytes = std::get<fastgltf::sources::Array>(buffer.data).bytes;
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(bytes.size()), bytes.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  return data;
}

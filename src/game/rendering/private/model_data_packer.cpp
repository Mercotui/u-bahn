// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/rendering/private/model_data_packer.h"

#include "platform/gl.h"

ModelData ModelDataPacker::PackBuffers(const fastgltf::Asset& asset) {
  ModelData data;
  glGenBuffers(1, &data.vertex_buffer_object);
  return data;
}

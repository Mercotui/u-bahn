// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <glm/mat4x4.hpp>

enum class Material {
  kSkybox,
  kPlain,
};

struct ModelData {
  unsigned vertex_array_object{};
  unsigned vertex_buffer_object{};
};

using ModelDataId = unsigned;

struct Model {
  ModelDataId data_id{};
  Material material;
  glm::mat4 transform;
};

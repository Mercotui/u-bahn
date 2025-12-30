// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <fastgltf/core.hpp>

#include "game/rendering/model.h"

class MeshPackerInterface {
 public:
  virtual ~MeshPackerInterface() = default;

  virtual ModelData PackBuffers(const fastgltf::Asset& asset) = 0;
};

class MeshPacker final : public MeshPackerInterface {
 public:
  ModelData PackBuffers(const fastgltf::Asset& asset) override;
};

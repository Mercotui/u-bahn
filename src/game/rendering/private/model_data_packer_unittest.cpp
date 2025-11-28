// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/rendering/private/model_data_packer.h"

#include <gtest/gtest.h>

TEST(ModelDataPackerTest, PackBuffers) {
  const auto asset = fastgltf::Asset();
  ModelData data = ModelDataPacker::PackBuffers(asset);
}

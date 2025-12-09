// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/rendering/private/model_cache.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <utility>

#include "game/rendering/private/gl_mock.h"
#include "utils/file_reader_interface.h"

namespace {
using ::testing::_;
using ::testing::InvokeArgument;

const auto kTestfileName = std::filesystem::path("test_cube.glb");
}  // namespace

class ModelCacheTest : public ::testing::Test {
 protected:
  GlMock gl_mock_;
};

TEST_F(ModelCacheTest, Unknown) {
  const ModelCache cache(std::move(FileReaderFactory::Create(FileReaderFactory::Type::kSimple)));
  const auto& data = cache.GetData(42);
  EXPECT_EQ(data.vertex_array_object, 0);
  EXPECT_EQ(data.vertex_buffer_object, 0);
}

TEST_F(ModelCacheTest, Load) {
  ModelCache cache(std::move(FileReaderFactory::Create(FileReaderFactory::Type::kSimple)));

  EXPECT_CALL(gl_mock_, Call(_, "glGenBuffers", _));

  const auto model = cache.Load(kTestfileName);
  EXPECT_EQ(model.material, Material::kPlain);
  EXPECT_GT(model.data_id, 0);  // const auto kTestfileName = std::filesystem::path("/home/menno/Downloads/Duck.glb");

  const auto& data = cache.GetData(model.data_id);
  // EXPECT_GT(data.vertex_array_object, 0);
  // EXPECT_GT(data.vertex_buffer_object, 0);
}

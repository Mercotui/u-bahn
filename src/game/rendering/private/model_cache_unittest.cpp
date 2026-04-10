// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/rendering/private/model_cache.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <utility>

#include "game/rendering/private/mesh_packer.h"
#include "utils/file_reader_interface.h"

namespace {
using ::testing::_;
using ::testing::DoAll;
using ::testing::InvokeArgument;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::WithArg;

const auto kTestfileName = std::filesystem::path("test_cube.glb");

class MeshPackerMock : public MeshPackerInterface {
 public:
  MOCK_METHOD(ModelData, PackBuffers, (const fastgltf::Asset& asset), (override));
};
}  // namespace

class ModelCacheTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto mesh_packer = std::make_unique<StrictMock<MeshPackerMock>>();

    // ReSharper disable once CppDFALocalValueEscapesFunction
    mesh_packer_mock_raw_ = mesh_packer.get();

    cache_ = std::make_unique<ModelCache>(std::move(FileReaderFactory::Create(FileReaderFactory::Type::kSimple)),
                                          std::move(mesh_packer));
  }

  MeshPackerMock* mesh_packer_mock_raw_{};
  std::unique_ptr<ModelCache> cache_{};
};

TEST_F(ModelCacheTest, Unknown) {
  const auto& data = cache_->GetData(42);
  EXPECT_EQ(data.vertex_array_object, 0);
  EXPECT_EQ(data.vertex_buffer_object, 0);
}

TEST_F(ModelCacheTest, Load) {
  constexpr auto kVaoName = 101;
  constexpr auto kVboName = 102;
  EXPECT_CALL(*mesh_packer_mock_raw_, PackBuffers(_))
      .WillOnce(DoAll(WithArg<0>([](const fastgltf::Asset& asset) {
                        ASSERT_EQ(asset.meshes.size(), 1);
                        EXPECT_EQ(asset.meshes[0].name, "Cube");
                      }),
                      Return(ModelData{kVaoName, kVboName})));
  const auto model = cache_->Register(kTestfileName);
  EXPECT_EQ(model.material, Material::kPlain);
  EXPECT_GT(model.data_id, 0);
  const auto& data = cache_->GetData(model.data_id);
  EXPECT_EQ(data.vertex_array_object, kVaoName);
  EXPECT_EQ(data.vertex_buffer_object, kVboName);
}

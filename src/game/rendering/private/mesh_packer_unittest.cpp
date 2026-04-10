// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/rendering/private/mesh_packer.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <utility>

#include "game/rendering/private/gl_mock.h"

namespace {
using ::testing::_;
using ::testing::Sequence;
using ::testing::WithArg;

constexpr GLuint kBufferHandle{3001};
constexpr GLuint kVertexArrayObjectHandle{3001};
const auto kTestfileName = std::filesystem::path("test_cube.gltf");
}  // namespace

class MeshPackerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    fastgltf::GltfFileStream file(kTestfileName);
    ASSERT_TRUE(file.isOpen()) << "Could not open GLTF example data.";
    fastgltf::Parser parser;
    asset_ = parser.loadGltf(file, ".");
    ASSERT_EQ(asset_.error(), fastgltf::Error::None) << "Could not load GLTF example data.";
  }

  fastgltf::Expected<fastgltf::Asset> asset_{fastgltf::Error::None};
  GlMock gl_mock_;
};

TEST_F(MeshPackerTest, PackBuffers) {
  Sequence seq;
  MeshPacker mesh_packer;

  EXPECT_CALL(gl_mock_, Call(_, "glGenVertexArrays", _)).WillOnce(WithArg<2>(+[](const GlMock::Args& gl_args) -> void {
    EXPECT_EQ(gl_args.at(0), GlMock::Arg(1));
    *get<GLuint*>(gl_args.at(1)) = kVertexArrayObjectHandle;
  }));
  EXPECT_CALL(gl_mock_, Call(_, "glGenBuffers", _)).WillOnce(WithArg<2>(+[](const GlMock::Args& gl_args) -> void {
    EXPECT_EQ(gl_args.at(0), GlMock::Arg(1));
    *get<GLuint*>(gl_args.at(1)) = kBufferHandle;
  }));
  EXPECT_CALL(gl_mock_, Call(_, "glBindVertexArray", _))
      .InSequence(seq)
      .WillOnce(WithArg<2>(+[](const GlMock::Args& gl_args) -> void {
        EXPECT_EQ(gl_args.at(0), GlMock::Arg(kVertexArrayObjectHandle));
      }));
  EXPECT_CALL(gl_mock_, Call(_, "glBindBuffer", _)).WillOnce(WithArg<2>(+[](const GlMock::Args& gl_args) -> void {
    EXPECT_EQ(gl_args.at(0), GlMock::Arg(std::in_place_type<GLuint>, GL_ARRAY_BUFFER));
    EXPECT_EQ(gl_args.at(1), GlMock::Arg(kBufferHandle));
  }));
  EXPECT_CALL(gl_mock_, Call(_, "glBufferData", _)).WillOnce(WithArg<2>(+[](const GlMock::Args& gl_args) -> void {
    EXPECT_EQ(gl_args.at(0), GlMock::Arg(std::in_place_type<GLuint>, GL_ARRAY_BUFFER));
  }));
  EXPECT_CALL(gl_mock_, Call(_, "glBindVertexArray", _))
      .InSequence(seq)
      .WillOnce(WithArg<2>(+[](const GlMock::Args& gl_args) -> void {
        EXPECT_EQ(gl_args.at(0), GlMock::Arg(std::in_place_type<GLuint>, 0));
      }));

  ModelData data = mesh_packer.PackBuffers(asset_.get());
  EXPECT_EQ(data.vertex_buffer_object, kBufferHandle);
}

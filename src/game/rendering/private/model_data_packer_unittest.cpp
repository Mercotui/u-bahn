// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/rendering/private/model_data_packer.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "game/rendering/private/gl_mock.h"

namespace {
using ::testing::_;
using ::testing::WithArg;

constexpr GLuint kBufferHandle{3001};
}  // namespace

class ModelDataPackerTest : public ::testing::Test {
 protected:
  GlMock gl_mock_;
};

TEST_F(ModelDataPackerTest, PackBuffers) {
  const auto asset = fastgltf::Asset();

  EXPECT_CALL(gl_mock_, Call(_, "glGenBuffers", _)).WillOnce(WithArg<2>(+[](const GlMock::Args& gl_args) -> void {
    EXPECT_EQ(gl_args.at(0), GlMock::Arg(1));
    *get<GLuint*>(gl_args.at(1)) = kBufferHandle;
  }));

  ModelData data = ModelDataPacker::PackBuffers(asset);
  EXPECT_EQ(data.vertex_buffer_object, kBufferHandle);
}

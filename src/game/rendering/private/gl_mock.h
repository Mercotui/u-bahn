// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <glad/gles2.h>
#include <gmock/gmock.h>

#include <vector>

/**
 * Mocks OpenGL via GLAD debug.
 *
 * This was written as a test, to find the limits of gmock and GLAD.
 * Now that I have seen the limits, I recommend to steer clear of them.
 * You shouldn't follow this approach, the general consensus is to write rendering code without unittests.
 */
class GlMock {
 public:
  using Arg = std::variant<GLint, GLint*, GLuint, GLuint*, GLbyte, GLbyte*, GLfloat, GLfloat*, GLchar*>;
  using Args = std::vector<Arg>;

  GlMock();
  ~GlMock();

  MOCK_METHOD(void, Call, (const void* ret, std::string_view, const Args& args), (const));
};

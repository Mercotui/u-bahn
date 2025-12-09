// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/rendering/private/gl_mock.h"

#include <absl/log/log.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdarg>

namespace {
#define CHECK_NAME(NAME) if (name == NAME) {
#define CHECK_END }
#define CHECK_CONTINUE CHECK_END else
#define PARSE_GL_ARG(TYPE) args.push_back(va_arg(args_list, TYPE));
#define PARSE_GL_ARGS1(NAME, ARG0) CHECK_NAME(NAME) PARSE_GL_ARG(ARG0) CHECK_CONTINUE
#define PARSE_GL_ARGS2(NAME, ARG0, ARG1) CHECK_NAME(NAME) PARSE_GL_ARG(ARG0) PARSE_GL_ARG(ARG1) CHECK_CONTINUE
#define PARSE_GL_ARGS3(NAME, ARG0, ARG1, ARG2) \
  CHECK_NAME(NAME) PARSE_GL_ARG(ARG0) PARSE_GL_ARG(ARG1) PARSE_GL_ARG(ARG2) CHECK_CONTINUE
#define PARSE_GL_ARGS4(NAME, ARG0, ARG1, ARG2, ARG3) \
  CHECK_NAME(NAME) PARSE_GL_ARG(ARG0) PARSE_GL_ARG(ARG1) PARSE_GL_ARG(ARG2) PARSE_GL_ARG(ARG3) CHECK_CONTINUE

using ::testing::_;
using ::testing::MockFunction;

/// Global instance used for testing, note only once mock can exist at a time
GlMock* GlobalGlMock{};

void glad_pre_call_callback(const char*, GLADapiproc, int, ...) {}
void glad_pre_call_callback_null(const char*, GLADapiproc, int, ...) {
  LOG(FATAL)
      << "Calling GL function without an active mock. GlMock was previously loaded, but has been destroyed already.";
}

void glad_post_call_callback(void* ret, const char* name_raw, GLADapiproc, const int len_args, ...) {
  const auto name = std::string_view{name_raw};
  GlMock::Args args;
  va_list args_list;
  va_start(args_list, len_args);
  PARSE_GL_ARGS1("glGetString", GLenum)
  PARSE_GL_ARGS2("glGetIntegerv", GLenum, GLint*)
  PARSE_GL_ARGS2("glGenBuffers", GLsizei, GLuint*)
  // Each PARSE_GL_ARGS ends with else, finish off the list with this catch all log:
  {  // NOLINT(whitespace/braces)
    LOG(WARNING) << "Unknown args for GL mock: " << name_raw;
  }
  va_end(args_list);

  GlobalGlMock->Call(ret, name, args);
}

/*
 * This section defines special cases of stubs, for setup requirements
 */
const GLubyte* glGetStringStub(const GLenum name) {
  if (name == GL_VERSION) {
    return reinterpret_cast<const GLubyte*>("OpenGL ES 3.0");
  }
  return nullptr;
}

void glGetIntegervStub(const GLenum pname, GLint* data) {
  if (pname == GL_NUM_EXTENSIONS) {
    *data = 0;
  }
}

/// Placeholder stub, does nothing
void glPlaceHolder() {}
}  // namespace

GlMock::GlMock() {
  if (!gladLoadGLES2(+[](const char* name_raw) -> GLADapiproc {
        const auto name = std::string_view(name_raw);
        if (name == "glGetString") {
          return reinterpret_cast<GLADapiproc>(&glGetStringStub);
        }
        if (name == "glGetIntegerv") {
          return reinterpret_cast<GLADapiproc>(&glGetIntegervStub);
        }
        // fairly hacky, but use the placeholder for all other OpenGL functions
        return &glPlaceHolder;
      })) {
    LOG(FATAL) << "Couldn't initialize GLAD mock";
  }
  gladSetGLES2PreCallback(glad_pre_call_callback);
  gladSetGLES2PostCallback(glad_post_call_callback);
  if (GlobalGlMock != nullptr) {
    LOG(FATAL) << "It appears a GlMock instance already exists";
  }
  GlobalGlMock = this;
}

GlMock::~GlMock() {
  // Tell GLAD to use a function that just asserts if some tries to call it.
  gladSetGLES2PreCallback(glad_pre_call_callback_null);
  GlobalGlMock = nullptr;
}

// SPDX-FileCopyrightText: Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
// SPDX-License-Identifier: MIT
// Adapted by Menno van der Graaf, original taken from here:
// https://github.com/danginsburg/opengles3-book/blob/master/Chapter_2/Hello_Triangle/Hello_Triangle.c

// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#include "third_party/hello_triangle/hello_triangle.h"

#include <absl/log/log.h>

#include <vector>

#include "platform/gl.h"

namespace {
GLuint programObject = 0;
GLuint vbo = 0;

GLuint LoadShader(GLenum type, const char *shaderSrc) {
  GLint compiled;
  const GLuint shader = glCreateShader(type);
  if (shader == 0) {
    return 0;
  }
  glShaderSource(shader, 1, &shaderSrc, nullptr);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    GLint infoLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen > 1) {
      std::vector<char> infoLog(infoLen);
      glGetShaderInfoLog(shader, infoLen, nullptr, infoLog.data());
      LOG(ERROR) << "Error compiling shader:\n" << infoLog.data();
    }
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}
}  // namespace

int HelloTriangle::Init() {
  constexpr char vertex_shader_source[] =
      "#version 300 es                          \n"
      "layout(location = 0) in vec4 vPosition;  \n"
      "void main()                              \n"
      "{                                        \n"
      "   gl_Position = vPosition;              \n"
      "}                                        \n";
  constexpr char fragment_shader_source[] =
      "#version 300 es                              \n"
      "precision mediump float;                     \n"
      "out vec4 fragColor;                          \n"
      "void main()                                  \n"
      "{                                            \n"
      "   fragColor = vec4 ( gl_FragCoord.x / 640.0, gl_FragCoord.y / 480.0, 0.0, 1.0 );  \n"
      "}                                            \n";
  GLint linked;
  const GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertex_shader_source);
  const GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragment_shader_source);
  programObject = glCreateProgram();
  if (programObject == 0) {
    return 0;
  }
  glAttachShader(programObject, vertexShader);
  glAttachShader(programObject, fragmentShader);
  glLinkProgram(programObject);
  glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
  if (!linked) {
    GLint infoLen = 0;
    glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen > 1) {
      std::vector<char> infoLog(infoLen);
      glGetProgramInfoLog(programObject, infoLen, nullptr, infoLog.data());
      LOG(ERROR) << "Error linking program:\n" << infoLog.data();
    }
    glDeleteProgram(programObject);
    return false;
  }
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  constexpr GLfloat vertices[] = {0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f};
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glViewport(0, 0, 640, 480);

  return true;
}

void HelloTriangle::Draw() {
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(programObject);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void HelloTriangle::Shutdown() {
  glDeleteProgram(programObject);
  glDeleteBuffers(1, &vbo);
}

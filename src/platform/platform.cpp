// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "platform/platform.h"

#include <SDL3/SDL.h>
#include <absl/log/log.h>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include <iomanip>
#include <memory>

#include "platform/gl.h"

namespace {
constexpr auto kPlatformType =
#if defined(PLATFORM_DESKTOP)
    Platform::Type::kDesktop;
#elif defined(PLATFORM_ANDROID)
    Platform::Type::kAndroid;
#elif defined(PLATFORM_WEB)
    Platform::Type::kWeb;
#endif

// TODO(Menno 15.10.2025) Delete these two defines
constexpr auto kWindowWidth = 640;
constexpr auto kWindowHeight = 480;

SDL_Window* CreateWindow() {
  SDL_SetAppMetadata("U-Bahn", "1.0", "com.mercotui.ubahn");

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    LOG(ERROR) << "Couldn't initialize SDL: " << SDL_GetError();
    return nullptr;
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  return SDL_CreateWindow("U-Bahn", kWindowWidth, kWindowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
}

SDL_GLContext CreateGlContext(SDL_Window* window) {
  const auto context = SDL_GL_CreateContext(window);
  if (!context) {
    LOG(ERROR) << "Couldn't create OpenGL context: " << SDL_GetError();
    return nullptr;
  }

  if (!SDL_GL_MakeCurrent(window, context)) {
    LOG(ERROR) << "Couldn't make OpenGL context current: " << SDL_GetError();
    SDL_GL_DestroyContext(context);
    return nullptr;
  }

#if !defined(PLATFORM_WEB)
  if (!gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
    LOG(ERROR) << "Couldn't initialize GLAD";
    SDL_GL_DestroyContext(context);
    return nullptr;
  }
#endif
  LOG(INFO) << "GLES Version: " << glGetString(GL_MAJOR_VERSION) << "." << glGetString(GL_MAJOR_VERSION);
  LOG(INFO) << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
  LOG(INFO) << "OpenGL Renderer: " << glGetString(GL_RENDERER);
  return context;
}
}  // namespace

Platform::~Platform() {
  if (gl_context_) {
    SDL_GL_DestroyContext(gl_context_);
  }
  if (window_) {
    SDL_DestroyWindow(window_);
  }
}

Platform::Type Platform::GetPlatform() { return kPlatformType; }

std::unique_ptr<Platform> Platform::Init() {
  auto self = std::make_unique<Platform>();

  self->window_ = CreateWindow();
  if (!self->window_) {
    LOG(ERROR) << "Couldn't create window: " << SDL_GetError();
    return nullptr;
  }

  self->gl_context_ = CreateGlContext(self->window_);
  if (!self->gl_context_) {
    return nullptr;
  }

  // We need to swap buffers once to show the window on Wayland
  SDL_GL_SwapWindow(self->window_);

  return self;
}

void Platform::Loop(MainLoop* main_loop) {
  main_loop->platform = this;

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(+[](void* loop) { LoopInternal(static_cast<MainLoop*>(loop)); }, main_loop, 0, true);
#else
  while (LoopInternal(main_loop)) {
  }
#endif

  main_loop->platform = nullptr;
}

bool Platform::LoopInternal(MainLoop* main_loop) {
  const auto self = main_loop->platform;

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
  }

  // Call the application loop
  const bool should_continue = (*main_loop)();

  SDL_GL_SwapWindow(self->window_);

  return should_continue;
}

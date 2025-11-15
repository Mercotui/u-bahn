// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <SDL3/SDL.h>

#include <memory>

class Platform {
 public:
  enum class Type { kAndroid, kDesktop, kWeb };
  struct MainLoop {
    virtual ~MainLoop() = default;
    virtual bool operator()() = 0;
    Platform* platform = nullptr;
  };

  static Type GetPlatform();

  // Init, get window and OpenGL-ES 3.0 context
  static std::unique_ptr<Platform> Init();

  // Clean up platform resources
  ~Platform();

  // Start loop, until shutdown is requested
  void Loop(MainLoop* main_loop);

 private:
  static bool LoopInternal(MainLoop* main_loop);

  SDL_Window* window_ = nullptr;
  SDL_GLContext gl_context_ = nullptr;
};

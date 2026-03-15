// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <SDL3/SDL.h>

#include <memory>

// TODO(Menno 13.03.2026) Leaking private include here, please hide behind interface
#include "platform/private/input_manager.h"

class Platform {
 public:
  enum class Type { kAndroid, kDesktop, kWeb };

  struct LoopContext {
    const Inputs& inputs;
  };

  struct MainLoop {
    virtual ~MainLoop() = default;
    virtual bool operator()(const LoopContext&) = 0;
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

  // TODO(Menno 09.03.2026) Hide these variables behind an PlatformInterface
  SDL_Window* window_ = nullptr;
  SDL_GLContext gl_context_ = nullptr;
  InputManager input_manager_;
};

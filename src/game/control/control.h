// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <string>

#include "game/input/input.h"

namespace Control {
struct Scheme {};

struct MenuControls {
  bool cursor_active_{};
  float cursor_x{};
  float cursor_y{};
};

struct TrainControls {
  float throttle{};
  float brake{};

  bool reverse{};

  bool doors_unlock_left{};
  bool doors_unlock_right{};
  bool doors_close{};
};

struct CameraControls {
  float x{};
  float y{};
};

struct GameControls {
  std::string input_name;
  Input::Type input_type{Input::Type::kKeyboard};

  // TODO(Menno 23.05.2024) Move this to the menu options.
  bool show_debug{};

  CameraControls camera_controls;
  TrainControls train_controls;
};
}  // namespace Control

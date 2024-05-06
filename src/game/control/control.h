#pragma once

#include <variant>

#include "game/input/input.h"

namespace Control {
struct Scheme {};

enum class Mode { kMenu, kTrain };

struct MenuControls {
  bool cursor_active_{};
  float cursor_x{};
  float cursor_y{};
};

struct TrainControls {
  Input::Type input_type;

  float throttle{};
  float brake{};

  bool reverse{};

  bool doors_unlock_left{};
  bool doors_unlock_right{};
  bool doors_close{};
};

using Controls = std::variant<MenuControls, TrainControls>;
};  // namespace Control
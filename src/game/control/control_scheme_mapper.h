#pragma once

#include "game/control/control.h"
#include "game/input/input.h"

class ControlSchemeMapper {
 public:
  Control::GameControls MapGameControls(const InputList& inputs);

 private:
  // TODO(Menno 16.02.2025) Break these variables out into mouse specific class
  float mouse_last_x_{};
  float mouse_last_y_{};
  // TODO(Menno 03.06.2024) break this touch specific variable out into a separate class
  float drag_start_{0};
  Control::Scheme control_scheme_{};
};

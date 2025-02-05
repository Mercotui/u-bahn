#pragma once

#include <memory>

#include "game/control/control.h"
#include "game/input/input.h"

class ControlSchemeMapper {
 public:
  Control::Controls Map(const InputList& inputs, Control::Mode);

 private:
  // TODO(Menno 03.06.2024) break this mouse specific variable out into a separate class
  float mouse_drag_start_{0};
  Control::Scheme control_scheme_{};
};

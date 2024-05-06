#pragma once

#include <memory>

#include "game/control/control.h"
#include "game/input/input.h"

class ControlSchemeMapper {
 public:
  Control::Controls Map(const InputList& inputs, Control::Mode);

 private:
  std::shared_ptr<Input> GetActiveInput(const InputList& inputs);

  Input::Type active_input_type_{};
  int active_input_id_{};
  Control::Scheme control_scheme_{};
};

#include "game/control/control_scheme_mapper.h"

namespace {
using Control::Controls;
using Control::MenuControls;
using Control::Mode;
using Control::TrainControls;
}  // namespace

Controls ControlSchemeMapper::Map(const InputList& inputs, Control::Mode mode) {
  auto input = GetActiveInput(inputs);

  switch (mode) {
    case Mode::kTrain: {
      if (input) {
        return TrainControls{.throttle = input->axes[2].value};
      } else {
        return TrainControls{};
      }
    }
    case Mode::kMenu: {
      return MenuControls();
    }
  }
}

std::shared_ptr<Input> ControlSchemeMapper::GetActiveInput(const std::vector<std::shared_ptr<Input>>& inputs) {
  for (const auto& input : inputs) {
    if (input->type == Input::Type::kJoystick && input->id == 1) {
      return input;
    }
  }

  return nullptr;
}

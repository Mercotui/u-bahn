#include "game/control/control_scheme_mapper.h"

#include "game/input/keyboard_mouse.h"

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
        if (input->type == Input::Type::kKeyboard) {
          return TrainControls{.throttle =
                                   input->buttons[static_cast<unsigned>(KeyboardMouseInput::Key::kW)].down   ? 1.0f
                                   : input->buttons[static_cast<unsigned>(KeyboardMouseInput::Key::kS)].down ? -1.0f
                                                                                                             : 0.0f};
        } else {
          return TrainControls{.throttle = input->axes[2].value};
        }
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
    //    if (input->type == Input::Type::kJoystick && input->id == 1) {
    if (input->type == Input::Type::kKeyboard) {
      return input;
    }
  }

  return nullptr;
}

#include "game/control/control_scheme_mapper.h"

#include "game/input/keyboard_mouse.h"

namespace {
using Control::Controls;
using Control::MenuControls;
using Control::Mode;
using Control::TrainControls;
using KeyboardMouseInput::Key;

bool DetectDownStroke(const Input::Button& button) { return button.changed && button.down; }

const auto& GetButton(const std::shared_ptr<Input>& input, KeyboardMouseInput::Key key) {
  return input->buttons[static_cast<unsigned>(key)];
}

}  // namespace

Controls ControlSchemeMapper::Map(const InputList& inputs, Control::Mode mode) {
  auto input = GetActiveInput(inputs);

  switch (mode) {
    case Mode::kTrain: {
      if (input) {
        if (input->type == Input::Type::kKeyboard) {
          return TrainControls{.throttle = GetButton(input, Key::kW).down ? 1.0f : 0.0f,
                               .brake = GetButton(input, Key::kS).down ? 1.0f : 0.0f,
                               .reverse = DetectDownStroke(GetButton(input, Key::kR)),
                               .show_debug = DetectDownStroke(GetButton(input, Key::kPeriod))};
        } else {
          return TrainControls{.throttle = input->axes[2].value};
        }
      } else {
        return TrainControls{};
      }
    }
    default:
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

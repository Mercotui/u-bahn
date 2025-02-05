#include "game/control/control_scheme_mapper.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "game/input/keyboard_mouse.h"

namespace {
using Control::Controls;
using Control::MenuControls;
using Control::Mode;
using Control::TrainControls;
using KeyboardMouseInput::Key;
using KeyboardMouseInput::MouseAxis;
using KeyboardMouseInput::MouseButton;

bool DetectDownStroke(const Input::Button& button) { return button.changed && button.down; }

const Input::Button& GetButton(const std::shared_ptr<Input>& input, Key key) {
  return input->buttons[static_cast<unsigned>(key)];
}

const Input::Button& GetButton(const std::shared_ptr<Input>& input, MouseButton button) {
  return input->buttons[static_cast<unsigned>(button)];
}

float GetAxisValue(const std::shared_ptr<Input>& input, MouseAxis axis) {
  return input->axes[static_cast<unsigned>(axis)].value;
}

std::shared_ptr<Input> GetActiveInput(const std::vector<std::shared_ptr<Input>>& inputs) {
  for (const auto& input : inputs) {
    if (input->active) {
      return input;
    }
  }

  return nullptr;
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
        } else if (input->type == Input::Type::kMouse) {
          auto& left_button = GetButton(input, MouseButton::kLeft);
          float throttle{};
          float brake{};
          if (left_button.changed && left_button.down) {
            mouse_drag_start_ = GetAxisValue(input, MouseAxis::kY);
          } else if (left_button.down) {
            float drag_diff = 0.01f * (mouse_drag_start_ - GetAxisValue(input, MouseAxis::kY));
            throttle = std::max(0.0f, drag_diff);
            brake = std::max(0.0f, -drag_diff);
          }
          return TrainControls{.throttle = throttle, .brake = brake};
        } else if (input->type == Input::Type::kTouch) {
          auto& button = input->buttons[0];
          float throttle{};
          float brake{};
          if (button.changed && button.down) {
            mouse_drag_start_ = input->axes[1].value;
          } else if (button.down) {
            float drag_diff = 0.01f * (mouse_drag_start_ - input->axes[1].value);
            throttle = std::max(0.0f, drag_diff);
            brake = std::max(0.0f, -drag_diff);
          }
          return TrainControls{.throttle = throttle, .brake = brake};
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

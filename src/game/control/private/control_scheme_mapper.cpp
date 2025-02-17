#include "game/control/control_scheme_mapper.h"

#include <absl/base/macros.h>

#include <algorithm>
#include <memory>
#include <vector>

#include "game/input/keyboard_mouse.h"

namespace {
using Control::CameraControls;
using Control::GameControls;
using Control::MenuControls;
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
    // TODO(Menno 06.02.2025) Merge all active inputs together so that moving the mouse does not block keyboard input
    if (input->active) {
      return input;
    }
  }

  return nullptr;
}
}  // namespace

GameControls ControlSchemeMapper::MapGameControls(const InputList& inputs) {
  GameControls controls;
  const auto input = GetActiveInput(inputs);

  if (!input) {
    return controls;
  }

  controls.input_name = input->name;
  controls.input_type = input->type;

  if (input->type == Input::Type::kKeyboard) {
    controls.show_debug = DetectDownStroke(GetButton(input, Key::kPeriod));
    controls.train_controls.throttle = GetButton(input, Key::kW).down ? 1.0f : 0.0f;
    controls.train_controls.brake = GetButton(input, Key::kS).down ? 1.0f : 0.0f;
    controls.train_controls.reverse = DetectDownStroke(GetButton(input, Key::kR));
  } else if (input->type == Input::Type::kMouse) {
    if (const auto& left_button = GetButton(input, MouseButton::kLeft); left_button.down) {
      const float new_x = GetAxisValue(input, MouseAxis::kX);
      const float new_y = GetAxisValue(input, MouseAxis::kY);
      if (!left_button.changed) {
        controls.camera_controls.x = new_x - mouse_last_x_;
        controls.camera_controls.y = new_y - mouse_last_y_;
      }
      mouse_last_x_ = new_x;
      mouse_last_y_ = new_y;
    }
  } else if (input->type == Input::Type::kTouch) {
    const auto& button = input->buttons[0];
    float throttle{};
    float brake{};
    if (button.changed && button.down) {
      drag_start_ = input->axes[1].value;
    } else if (button.down) {
      const float drag_diff = 0.01f * (drag_start_ - input->axes[1].value);
      throttle = std::max(0.0f, drag_diff);
      brake = std::max(0.0f, -drag_diff);
    }
    controls.train_controls.throttle = throttle;
    controls.train_controls.brake = brake;
  } else if (input->type == Input::Type::kJoystick) {
    controls.train_controls.throttle = input->axes[2].value;
  } else {
    // Unknown input type
    ABSL_ASSERT(false);
  }

  return controls;
}

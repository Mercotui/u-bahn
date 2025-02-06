#include "game/input/private/raylib_input_handlers.h"

#include <absl/log/log.h>
#include <raylib.h>

#include "game/input/input.h"
#include "game/input/private/raylib_input_helpers.h"

namespace {
constexpr float kMouseActivityDetectionScale = 0.005;
}  // namespace

namespace RaylibInputHandlers {
Keyboard::Keyboard() : input_(std::make_shared<Input>()) {
  input_->name = "Keyboard";
  input_->type = Input::Type::kKeyboard;
  input_->buttons.resize(static_cast<unsigned>(KeyboardMouseInput::Key::kEnumKeySize), {});

  unsigned count{0};
  std::ranges::for_each(input_->buttons, [&count](auto& button) {
    button.name = KeyboardMouseInput::KeyName(static_cast<KeyboardMouseInput::Key>(count++));
  });
}

InputList Keyboard::Poll() {
  input_->active = false;

  unsigned count{0};
  std::ranges::for_each(input_->buttons, [&count, this](Input::Button& button) {
    const auto key_code = static_cast<KeyboardMouseInput::Key>(count++);
    const KeyboardKey raylib_key_code = RaylibInputHelpers::RaylibKeyboardKey(key_code);

    const bool is_down = IsKeyDown(raylib_key_code);
    button.changed = is_down != button.down;
    button.down = is_down;

    input_->active |= button.down;
  });

  return {input_};
}

Mouse::Mouse() : input_(std::make_shared<Input>()) {
  input_->name = "Mouse";
  input_->type = Input::Type::kMouse;

  input_->buttons.resize(static_cast<unsigned>(KeyboardMouseInput::MouseButton::kEnumMouseButtonSize), {});
  unsigned button_count{0};
  std::ranges::for_each(input_->buttons, [&button_count](auto& button) {
    button.name = KeyboardMouseInput::MouseButtonName(static_cast<KeyboardMouseInput::MouseButton>(button_count++));
  });

  constexpr auto number_of_axes = static_cast<unsigned>(KeyboardMouseInput::MouseAxis::kEnumMouseAxisSize);
  axis_activity_.resize(number_of_axes);
  input_->axes.resize(number_of_axes, {});
  unsigned axis_count{0};
  std::ranges::for_each(input_->axes, [&axis_count](auto& axis) {
    axis.name = KeyboardMouseInput::MouseAxisName(static_cast<KeyboardMouseInput::MouseAxis>(axis_count++));
  });
}

InputList Mouse::Poll() {
  input_->active = false;

  unsigned button_count{0};
  std::ranges::for_each(input_->buttons, [&button_count, this](Input::Button& button) {
    const auto button_idx = static_cast<KeyboardMouseInput::MouseButton>(button_count++);
    if (const auto optional_raylib_button_idx = RaylibInputHelpers::RaylibMouseButton(button_idx)) {
      // TODO(Menno 03.06.2024) IsMouseButton down might miss clicks if they happen inbetween frames?
      const bool is_down = IsMouseButtonDown(optional_raylib_button_idx.value());
      button.changed = is_down != button.down;
      button.down = is_down;
      input_->active |= button.down;
    }
  });

  // Use the same time_point for all axes, which might produce some time variance because we don't take a single
  // snapshot of all values. But I don't expect that to be an issue.
  const auto time_point = std::chrono::steady_clock::now();
  const auto update_axis = [this, &time_point](const KeyboardMouseInput::MouseAxis axis, const float value) {
    const auto axis_index = static_cast<unsigned>(axis);
    auto& axis_ref = input_->axes[axis_index];
    axis_ref.value = value;

    const float activity_value = value * kMouseActivityDetectionScale;
    axis_ref.active = axis_activity_[axis_index].Detect({.time_point = time_point, .value = activity_value});
    input_->active |= axis_ref.active;
  };

  update_axis(KeyboardMouseInput::MouseAxis::kX, static_cast<float>(GetMouseX()));
  update_axis(KeyboardMouseInput::MouseAxis::kY, static_cast<float>(GetMouseY()));
  update_axis(KeyboardMouseInput::MouseAxis::kWheel, GetMouseWheelMove());

  return {input_};
}

Touch::Touch() : input_(std::make_shared<Input>()) {
  input_->name = "Touch";
  input_->type = Input::Type::kTouch;

  input_->buttons.push_back({.name = "Touch 1"});
  input_->axes.push_back({.name = "Touch Axis Horizontal"});
  input_->axes.push_back({.name = "Touch Axis Vertical"});
}

InputList Touch::Poll() {
  if (GetTouchPointCount() > 0) {
    auto& button_ref = input_->buttons[0];
    // changed only true if previously not down
    button_ref.changed = !button_ref.down;
    button_ref.down = true;

    input_->axes[0].value = static_cast<float>(GetTouchX());
    input_->axes[1].value = static_cast<float>(GetTouchY());
  } else {
    auto& button_ref = input_->buttons[0];
    // changed only true if previously down
    button_ref.changed = button_ref.down;
    button_ref.down = false;

    input_->axes[0].value = 0.0f;
    input_->axes[1].value = 0.0f;
  }

  input_->active = input_->buttons[0].down;
  return {input_};
}
}  // namespace RaylibInputHandlers

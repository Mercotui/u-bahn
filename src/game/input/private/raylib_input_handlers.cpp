#include "game/input/private/raylib_input_handlers.h"

#include <absl/log/log.h>
#include <raylib.h>

#include "game/input/input.h"
#include "game/input/private/raylib_input_helpers.h"

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

    if (button.down) {
      input_->active = true;
    }
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

  input_->axes.resize(static_cast<unsigned>(KeyboardMouseInput::MouseAxis::kEnumMouseAxisSize), {});
  unsigned axis_count{0};
  for_each(std::begin(input_->axes), std::end(input_->axes), [&axis_count](auto& axis) {
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
      if (button.down) {
        input_->active = true;
      }
    }
  });

  const auto get_axis_ref = [this](KeyboardMouseInput::MouseAxis axis) -> Input::Axis& {
    return input_->axes[static_cast<unsigned>(axis)];
  };

  get_axis_ref(KeyboardMouseInput::MouseAxis::kX).value = static_cast<float>(GetMouseX());
  get_axis_ref(KeyboardMouseInput::MouseAxis::kY).value = static_cast<float>(GetMouseY());
  get_axis_ref(KeyboardMouseInput::MouseAxis::kWheel).value = GetMouseWheelMove();

  // TODO(Menno 03.02.2025) Detect mouse axis activity if buttons are not pressed
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

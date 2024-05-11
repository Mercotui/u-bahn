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
  for_each(std::begin(input_->buttons), std::end(input_->buttons), [&count](auto& button) {
    button.name = KeyboardMouseInput::KeyName(static_cast<KeyboardMouseInput::Key>(count++));
  });
}

InputList Keyboard::Poll() {
  unsigned count{0};
  for_each(std::begin(input_->buttons), std::end(input_->buttons), [&count](Input::Button& button) {
    auto key_code = static_cast<KeyboardMouseInput::Key>(count++);
    KeyboardKey raylib_key_code = RaylibInputHelpers::RaylibKeyboardKey(key_code);

    bool is_down = IsKeyDown(raylib_key_code);
    button.changed = is_down != button.down;
    button.down = is_down;
  });

  return {input_};
}

Mouse::Mouse() : input_(std::make_shared<Input>()) {
  input_->name = "Mouse";
  input_->type = Input::Type::kMouse;
}

InputList Mouse::Poll() { return {input_}; }

Touch::Touch() : input_(std::make_shared<Input>()) {
  input_->name = "Touch";
  input_->type = Input::Type::kTouch;
}

InputList Touch::Poll() { return {input_}; }
}  // namespace RaylibInputHandlers

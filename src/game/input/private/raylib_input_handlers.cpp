#include "game/input/private/raylib_input_handlers.h"

#include <absl/log/log.h>
#include <raylib.h>

#include "game/input/input.h"

namespace RaylibInputHandlers {
Keyboard::Keyboard() : input_(std::make_shared<Input>()) {
  input_->name = "Keyboard";
  input_->type = Input::Type::kKeyboard;
}

InputList Keyboard::Poll() { return {input_}; }

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

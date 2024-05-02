#include "game/input/private/raylib_input_handlers.h"

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

Gamepad::Gamepad() : input_() {}

InputList Gamepad::Poll() { return {}; }

void Gamepad::SetConfig(int id, Input::Config) {}
}  // namespace RaylibInputHandlers
#include "game/input/private/raylib_input_handlers.h"

#include "game/input/input.h"

namespace RaylibInputHandlers {
std::vector<Input::Info> Keyboard::ListInputs() const {
  return {
      Input::Info({
          .name = "Keyboard",
          .type = Input::Type::kKeyboard,
      }),
  };
}

void Keyboard::Poll() {}

void Keyboard::SetConfig(int id, Input::Config) {}

std::vector<Input::Info> Mouse::ListInputs() const {
  return {
      Input::Info({
          .name = "Mouse",
          .type = Input::Type::kMouse,
      }),
  };
}

void Mouse::Poll() {}

void Mouse::SetConfig(int id, Input::Config) {}

std::vector<Input::Info> Touch::ListInputs() const {
  return {
      Input::Info({
          .name = "Touch",
          .type = Input::Type::kTouch,
      }),
  };
}

void Touch::Poll() {}

void Touch::SetConfig(int id, Input::Config) {}

std::vector<Input::Info> Gamepad::ListInputs() const { return {}; }

void Gamepad::Poll() {}

void Gamepad::SetConfig(int id, Input::Config) {}
}  // namespace RaylibInputHandlers
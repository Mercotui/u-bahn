#include "game/input/private/input_manager.h"

#include <absl/log/log.h>

#include <iostream>

#include "game/input/input.h"
#include "game/input/private/joystick_handler.h"
#include "game/input/private/raylib_input_handlers.h"
#include "platform/platform.h"

namespace InputManagerFactory {
std::unique_ptr<InputManagerInterface> Create(Platform::Type platform) {
  InputManager::DeviceHandlers handlers;
  switch (platform) {
    case Platform::Type::kAndroid: {
      handlers.emplace(Input::Type::kTouch, std::make_unique<RaylibInputHandlers::Touch>());
      break;
    }
    case Platform::Type::kWeb: {
      handlers.emplace(Input::Type::kTouch, std::make_unique<RaylibInputHandlers::Touch>());
      handlers.emplace(Input::Type::kKeyboard, std::make_unique<RaylibInputHandlers::Keyboard>());
      handlers.emplace(Input::Type::kMouse, std::make_unique<RaylibInputHandlers::Mouse>());
      break;
    }
    case Platform::Type::kDesktop: {
      handlers.emplace(Input::Type::kKeyboard, std::make_unique<RaylibInputHandlers::Keyboard>());
      handlers.emplace(Input::Type::kMouse, std::make_unique<RaylibInputHandlers::Mouse>());
      handlers.emplace(Input::Type::kJoystick, std::make_unique<JoystickHandler>());
      break;
    }
  }

  return std::make_unique<InputManager>(std::move(handlers));
}
}  // namespace InputManagerFactory

InputList InputManager::Poll() {
  InputList aggregated_inputs;
  for (const auto& [type, handler] : device_handlers_) {
    auto handler_inputs = handler->Poll();
    aggregated_inputs.insert(std::end(aggregated_inputs), std::make_move_iterator(std::begin(handler_inputs)),
                             std::make_move_iterator(std::end(handler_inputs)));
  }
  return aggregated_inputs;
}

void InputManager::SetConfig(int id, Input::Config) {
  std::cout << "InputManager::SetConfig unimplemented!" << std::endl;
}

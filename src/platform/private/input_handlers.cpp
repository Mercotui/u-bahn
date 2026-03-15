// SPDX-FileCopyrightText: 2026 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#include "platform/private/input_handlers.h"

#include <SDL3/SDL_events.h>
#include <absl/log/log.h>

#include "platform/input.h"
#include "platform/private/input_helpers.h"

void KeyboardHandler::Add(Inputs* inputs, const SDL_KeyboardDeviceEvent& event) {
  using KeyboardMouseInput::Key;
  using KeyboardMouseInput::KeyName;

  auto& keyboards = inputs->at(std::to_underlying(Input::Type::kKeyboard));
  auto& keyboard = keyboards.emplace_back();

  keyboard.id = event.which;
  keyboard.name = std::format("Keyboard {}", keyboard.id);
  keyboard.type = Input::Type::kKeyboard;
  keyboard.buttons.resize(static_cast<unsigned>(Key::kEnumKeySize), {});

  unsigned count{0};
  std::ranges::for_each(keyboard.buttons, [&count](auto& button) { button.name = KeyName(static_cast<Key>(count++)); });
}

void KeyboardHandler::Remove(Inputs* inputs, const SDL_KeyboardDeviceEvent& event) {
  auto& keyboards = inputs->at(std::to_underlying(Input::Type::kKeyboard));
  std::erase_if(keyboards, [&event](const auto& entry) { return entry.id == event.which; });
}

void KeyboardHandler::SetKey(Inputs* inputs, const SDL_KeyboardEvent& event) {
  if (event.repeat) {
    return;
  }

  auto& keyboards = inputs->at(std::to_underlying(Input::Type::kKeyboard));
  const auto keyboard =
      std::ranges::find_if(keyboards, [&event](const auto& entry) { return entry.id == event.which; });
  if (keyboard == keyboards.end()) {
    LOG(ERROR) << "Trying to set key=" << event.key << " to down=" << event.down
               << " for unknown keyboard=" << event.which;
    return;
  }

  auto& button = keyboard->buttons[std::to_underlying(InputSdlHelpers::ToKey(event.scancode))];
  button.changed = true;
  button.down = event.down;
  keyboard->active |= button.down;
}

void MouseHandler::Add(Inputs* inputs, const SDL_MouseDeviceEvent& event) {
  using KeyboardMouseInput::MouseAxis;
  using KeyboardMouseInput::MouseAxisName;
  using KeyboardMouseInput::MouseButton;
  using KeyboardMouseInput::MouseButtonName;

  auto& mice = inputs->at(std::to_underlying(Input::Type::kMouse));
  auto& mouse = mice.emplace_back();

  mouse.id = event.which;
  mouse.name = std::format("Mouse {}", mouse.id);
  mouse.type = Input::Type::kMouse;

  mouse.buttons.resize(static_cast<unsigned>(MouseButton::kEnumMouseButtonSize), {});
  unsigned button_count{0};
  std::ranges::for_each(mouse.buttons, [&button_count](auto& button) {
    button.name = MouseButtonName(static_cast<MouseButton>(button_count++));
  });

  constexpr auto number_of_axes = static_cast<unsigned>(MouseAxis::kEnumMouseAxisSize);
  mouse.axes.resize(number_of_axes, {});
  unsigned axis_count{0};
  std::ranges::for_each(mouse.axes,
                        [&axis_count](auto& axis) { axis.name = MouseAxisName(static_cast<MouseAxis>(axis_count++)); });
}

void MouseHandler::Remove(Inputs* inputs, const SDL_MouseDeviceEvent& event) {
  auto& mice = inputs->at(std::to_underlying(Input::Type::kMouse));
  std::erase_if(mice, [&event](const auto& entry) { return entry.id == event.which; });
}

void MouseHandler::SetPosition(Inputs* inputs, const SDL_MouseMotionEvent& event) {
  auto& mice = inputs->at(std::to_underlying(Input::Type::kMouse));
  const auto mouse = std::ranges::find_if(mice, [&event](const auto& entry) { return entry.id == event.which; });
  if (mouse == mice.end()) {
    return;
  }

  const auto timestamp = Clock::time_point(Clock::duration{event.timestamp});
  InputAxisHelpers::Assign(&mouse->axes[std::to_underlying(KeyboardMouseInput::MouseAxis::kX)], {timestamp, event.x});
  InputAxisHelpers::Assign(&mouse->axes[std::to_underlying(KeyboardMouseInput::MouseAxis::kY)], {timestamp, event.y});
}

void MouseHandler::SetWheel(Inputs* inputs, const SDL_MouseWheelEvent& event) {
  auto& mice = inputs->at(std::to_underlying(Input::Type::kMouse));
  const auto mouse = std::ranges::find_if(mice, [&event](const auto& entry) { return entry.id == event.which; });
  if (mouse == mice.end()) {
    return;
  }

  const auto timestamp = Clock::time_point(Clock::duration{event.timestamp});
  InputAxisHelpers::Assign(&mouse->axes[std::to_underlying(KeyboardMouseInput::MouseAxis::kWheel)],
                           {timestamp, event.y});
}

void MouseHandler::SetButton(Inputs* inputs, const SDL_MouseButtonEvent& event) {
  auto& mice = inputs->at(std::to_underlying(Input::Type::kMouse));
  const auto mouse = std::ranges::find_if(mice, [&event](const auto& entry) { return entry.id == event.which; });
  if (mouse == mice.end()) {
    return;
  }

  auto& button = mouse->buttons[std::to_underlying(InputSdlHelpers::ToMouseButton(event.button))];
  button.changed = true;
  button.down = event.down;
  mouse->active |= button.down;
}

// SPDX-FileCopyrightText: 2026 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#include "platform/private/input_manager.h"

#include <SDL3/SDL_events.h>
#include <absl/log/log.h>

#include <ranges>

#include "platform/input.h"
#include "platform/private/input_handlers.h"

namespace {
bool HandleKeyboardEvent(const SDL_Event& event, Inputs* inputs) {
  switch (event.type) {
    case SDL_EVENT_KEY_DOWN: [[fallthrough]];
    case SDL_EVENT_KEY_UP: {
      KeyboardHandler::SetKey(inputs, event.key);
      return true;
    }
    case SDL_EVENT_KEYBOARD_ADDED: {
      KeyboardHandler::Add(inputs, event.kdevice);
      return true;
    }
    case SDL_EVENT_KEYBOARD_REMOVED: {
      KeyboardHandler::Remove(inputs, event.kdevice);
      return true;
    }
    default: {
      return false;
    }
  }
}

bool HandleMouseEvent(const SDL_Event& event, Inputs* inputs) {
  switch (event.type) {
    case SDL_EVENT_MOUSE_MOTION: {
      MouseHandler::SetPosition(inputs, event.motion);
      return true;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN: [[fallthrough]];
    case SDL_EVENT_MOUSE_BUTTON_UP: {
      MouseHandler::SetButton(inputs, event.button);
      return true;
    }
    case SDL_EVENT_MOUSE_WHEEL: {
      MouseHandler::SetWheel(inputs, event.wheel);
      return true;
    }
    case SDL_EVENT_MOUSE_ADDED: {
      MouseHandler::Add(inputs, event.mdevice);
      return true;
    }
    case SDL_EVENT_MOUSE_REMOVED: {
      MouseHandler::Remove(inputs, event.mdevice);
      return true;
    }
    default: {
      return false;
    }
  }
}

bool HandleGamepadEvent(const SDL_Event& event, Inputs* inputs) {
  // TODO(Menno 15.03.2026) Add gamepad support
  switch (event.type) {
    case SDL_EVENT_GAMEPAD_ADDED: [[fallthrough]];
    case SDL_EVENT_GAMEPAD_REMOVED: [[fallthrough]];
    case SDL_EVENT_GAMEPAD_AXIS_MOTION: [[fallthrough]];
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN: [[fallthrough]];
    case SDL_EVENT_GAMEPAD_BUTTON_UP: [[fallthrough]];
    case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN: [[fallthrough]];
    case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: [[fallthrough]];
    case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: {
      return true;
    }
    default: {
      return false;
    }
  }
}

bool HandleTouchEvent(const SDL_Event& event, Inputs* inputs) {
  // TODO(Menno 15.03.2026) Add touch support
  switch (event.type) {
    case SDL_EVENT_FINGER_DOWN: [[fallthrough]];
    case SDL_EVENT_FINGER_UP: [[fallthrough]];
    case SDL_EVENT_FINGER_MOTION: [[fallthrough]];
    case SDL_EVENT_FINGER_CANCELED: {
      return true;
    }
    default: {
      return false;
    }
  }
}
}  // namespace

const Inputs& InputManager::GetInputs() { return inputs_; }

bool InputManager::HandleEvent(const SDL_Event& event) {
  return HandleKeyboardEvent(event, &inputs_) || HandleMouseEvent(event, &inputs_) ||
         HandleGamepadEvent(event, &inputs_) || HandleTouchEvent(event, &inputs_);
}

void InputManager::ClearChanges() {
  for (auto& input : inputs_ | std::ranges::views::join) {
    for (auto& button : input.buttons) {
      button.changed = false;
    }
  }
}

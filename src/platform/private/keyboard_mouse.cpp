// SPDX-FileCopyrightText: 2026 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#include "platform/keyboard_mouse.h"

#include <SDL3/SDL_keyboard.h>
#include <absl/log/log.h>

#include <format>
#include <string>
#include <type_traits>

#include "platform/private/input_helpers.h"

std::string KeyboardMouseInput::KeyName(const Key key) {
  const auto sdl_scancode = InputSdlHelpers::FromKey(key);
  const auto sdl_key = SDL_GetKeyFromScancode(sdl_scancode, SDL_KMOD_NONE, false);
  return SDL_GetKeyName(sdl_key);
}

std::string KeyboardMouseInput::MouseButtonName(MouseButton button) {
  switch (button) {
    case MouseButton::kLeft: {
      return "Mouse Button Left";
    }
    case MouseButton::kRight: {
      return "Mouse Button Right";
    }
    case MouseButton::kMiddle: {
      return "Mouse Button Middle";
    }
    case MouseButton::kForward: {
      return "Mouse Button Forward";
    }
    case MouseButton::kBack: {
      return "Mouse Button Back";
    }
    default: {
      return std::format("Unknown Button {}", static_cast<std::underlying_type<MouseButton>::type>(button));
    }
  }
}

std::string KeyboardMouseInput::MouseAxisName(MouseAxis axis) {
  switch (axis) {
    case MouseAxis::kX: {
      return "Mouse Axis Horizontal";
    }
    case MouseAxis::kY: {
      return "Mouse Axis Vertical";
    }
    case MouseAxis::kWheel: {
      return "Mouse Wheel";
    }
    default: {
      return std::format("Unknown Axis {}", static_cast<std::underlying_type<MouseAxis>::type>(axis));
    }
  }
}

// SPDX-FileCopyrightText: 2026 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>

#include <string>
#include <utility>

#include "platform/input.h"
#include "platform/keyboard_mouse.h"

namespace InputSdlHelpers {
KeyboardMouseInput::Key ToKey(SDL_Scancode scancode);
SDL_Scancode FromKey(KeyboardMouseInput::Key key);

KeyboardMouseInput::MouseButton ToMouseButton(SDL_MouseButtonFlags button);
}  // namespace InputSdlHelpers

namespace InputAxisHelpers {
/**
 * Assign a new value, and detect activity
 * @param axis axis to track
 * @param new_sample new value to assign
 * @return if activity has occurred
 */
bool Assign(Input::Axis* axis, Input::Axis::Sample new_sample);
}  // namespace InputAxisHelpers

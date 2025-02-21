// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <map>
#include <optional>

#include "game/input/keyboard_mouse.h"
#include "third_party/raylib/raylib.h"

namespace RaylibInputHelpers {
//! Lookup table for keycodes.
Raylib::KeyboardKey RaylibKeyboardKey(KeyboardMouseInput::Key key);

//! Lookup table for mouse button codes.
std::optional<Raylib::MouseButton> RaylibMouseButton(KeyboardMouseInput::MouseButton button);
}  // namespace RaylibInputHelpers

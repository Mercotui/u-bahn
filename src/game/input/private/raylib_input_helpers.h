#pragma once

#include <raylib.h>

#include <map>
#include <optional>
#include <string>

#include "game/input/keyboard_mouse.h"

namespace RaylibInputHelpers {

//! Lookup table for keycodes.
KeyboardKey RaylibKeyboardKey(KeyboardMouseInput::Key key);

//! Lookup table for mouse button codes.
std::optional<MouseButton> RaylibMouseButton(KeyboardMouseInput::MouseButton button);
}  // namespace RaylibInputHelpers

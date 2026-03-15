// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <string>

namespace KeyboardMouseInput {
enum class Key : unsigned {
  kUnspecified = 0,  ///< Non-valid key

  kA,  ///< Key: A | a
  kB,  ///< Key: B | b
  kC,  ///< Key: C | c
  kD,  ///< Key: D | d
  kE,  ///< Key: E | e
  kF,  ///< Key: F | f
  kG,  ///< Key: G | g
  kH,  ///< Key: H | h
  kI,  ///< Key: I | i
  kJ,  ///< Key: J | j
  kK,  ///< Key: K | k
  kL,  ///< Key: L | l
  kM,  ///< Key: M | m
  kN,  ///< Key: N | n
  kO,  ///< Key: O | o
  kP,  ///< Key: P | p
  kQ,  ///< Key: Q | q
  kR,  ///< Key: R | r
  kS,  ///< Key: S | s
  kT,  ///< Key: T | t
  kU,  ///< Key: U | u
  kV,  ///< Key: V | v
  kW,  ///< Key: W | w
  kX,  ///< Key: X | x
  kY,  ///< Key: Y | y
  kZ,  ///< Key: Z | z

  k1,  ///< Key: 1
  k2,  ///< Key: 2
  k3,  ///< Key: 3
  k4,  ///< Key: 4
  k5,  ///< Key: 5
  k6,  ///< Key: 6
  k7,  ///< Key: 7
  k8,  ///< Key: 8
  k9,  ///< Key: 9
  k0,  ///< Key: 0

  kEnter,         ///< Key: Enter
  kEscape,        ///< Key: Esc
  kBackspace,     ///< Key: Backspace
  kTab,           ///< Key: Tab
  kSpace,         ///< Key: Space
  kMinus,         ///< Key: -
  kEqual,         ///< Key: ,
  kLeftBracket,   ///< Key: [
  kRightBracket,  ///< Key: ]
  kBackslash,     ///< Key: '\'
  kSemicolon,     ///< Key: ;
  kApostrophe,    ///< Key: '
  kGrave,         ///< Key: `
  kComma,         ///< Key: ,
  kPeriod,        ///< Key: .
  kSlash,         ///< Key: /

  kCapsLock,  ///< Key: Caps lock

  kF1,   ///< Key: F1
  kF2,   ///< Key: F2
  kF3,   ///< Key: F3
  kF4,   ///< Key: F4
  kF5,   ///< Key: F5
  kF6,   ///< Key: F6
  kF7,   ///< Key: F7
  kF8,   ///< Key: F8
  kF9,   ///< Key: F9
  kF10,  ///< Key: F10
  kF11,  ///< Key: F11
  kF12,  ///< Key: F12

  kPrintScreen,  ///< Key: Print screen
  kScrollLock,   ///< Key: Scroll down
  kPause,        ///< Key: Pause
  kInsert,       ///< Key: Ins

  kHome,      ///< Key: Home
  kPageUp,    ///< Key: Page up
  kDelete,    ///< Key: Del
  kEnd,       ///< Key: End
  kPageDown,  ///< Key: Page down
  kRight,     ///< Key: Cursor right
  kLeft,      ///< Key: Cursor left
  kDown,      ///< Key: Cursor down
  kUp,        ///< Key: Cursor up

  kNumLock,  ///< Key: Num lock

  kKeypadDivide,    ///< Key: Keypad /
  kKeypadMultiply,  ///< Key: Keypad *
  kKeypadSubtract,  ///< Key: Keypad -
  kKeypadAdd,       ///< Key: Keypad +
  kKeypadEnter,     ///< Key: Keypad Enter
  kKeypadEqual,     ///< Key: Keypad =
  kKeypadDecimal,   ///< Key: Keypad .
  kKeypad1,         ///< Key: Keypad 1
  kKeypad2,         ///< Key: Keypad 2
  kKeypad3,         ///< Key: Keypad 3
  kKeypad4,         ///< Key: Keypad 4
  kKeypad5,         ///< Key: Keypad 5
  kKeypad6,         ///< Key: Keypad 6
  kKeypad7,         ///< Key: Keypad 7
  kKeypad8,         ///< Key: Keypad 8
  kKeypad9,         ///< Key: Keypad 9
  kKeypad0,         ///< Key: Keypad 0

  kLeftShift,     ///< Key: Shift left
  kLeftControl,   ///< Key: Control left
  kLeftAlt,       ///< Key: Alt left
  kLeftSuper,     ///< Key: Super left
  kRightShift,    ///< Key: Shift right
  kRightControl,  ///< Key: Control right
  kRightAlt,      ///< Key: Alt right
  kRightSuper,    ///< Key: Super right
  kMenu,          ///< Key: KB menu

  // Meta data
  kEnumKeySize  ///< size of Key enum
};

enum class MouseButton : unsigned {
  kUnspecified = 0,  ///< Non-valid button

  kLeft,     ///< Mouse button left
  kRight,    ///< Mouse button right
  kMiddle,   ///< Mouse button middle (pressed wheel)
  kForward,  ///< Mouse button forward (advanced mouse device)
  kBack,     ///< Mouse button back (advanced mouse device)

  // Meta data
  kEnumMouseButtonSize  ///< size of MouseButton enum
};

enum class MouseAxis : unsigned {
  kUnspecified = 0,  ///< Non-valid axis

  kX,      ///< Mouse axis horizontal
  kY,      ///< Mouse axis vertical
  kWheel,  ///< Mouse wheel

  // Meta data
  kEnumMouseAxisSize  ///< size of MouseAxis enum
};

/**
 * Lookup the name of a key
 * @param key the key to find the name for
 * @return the name of the key
 */
std::string KeyName(Key key);

/**
 * Lookup the name of a mouse button
 * @param button the button to find the name for
 * @return the name of the mouse button
 */
std::string MouseButtonName(MouseButton button);

/**
 * Lookup the name of a mouse axis
 * @param axis the axis to find the name for
 * @return the name of the mouse axis
 */
std::string MouseAxisName(MouseAxis axis);
}  // namespace KeyboardMouseInput

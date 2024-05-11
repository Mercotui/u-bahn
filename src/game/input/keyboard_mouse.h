#pragma once

#include <string>

namespace KeyboardMouseInput {
enum class Key : unsigned {
  kUnspecified = 0,  ///< Non-valid key

  // Special keys
  kLeftBracket,   ///< Key: [
  kRightBracket,  ///< Key: ]
  kGrave,         ///< Key: `
  kApostrophe,    ///< Key: '
  kComma,         ///< Key: ,
  kMinus,         ///< Key: -
  kPeriod,        ///< Key: .
  kSlash,         ///< Key: /
  kBackslash,     ///< Key: '\'
  kSemicolon,     ///< Key: ;
  kEqual,         ///< Key: ,

  // Alphanumeric keys
  k0,  ///< Key: 0
  k1,  ///< Key: 1
  k2,  ///< Key: 2
  k3,  ///< Key: 3
  k4,  ///< Key: 4
  k5,  ///< Key: 5
  k6,  ///< Key: 6
  k7,  ///< Key: 7
  k8,  ///< Key: 8
  k9,  ///< Key: 9
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

  // Function keys
  kSpace,         ///< Key: Space
  kEscape,        ///< Key: Esc
  kEnter,         ///< Key: Enter
  kTab,           ///< Key: Tab
  kBackspace,     ///< Key: Backspace
  kInsert,        ///< Key: Ins
  kDelete,        ///< Key: Del
  kRight,         ///< Key: Cursor right
  kLeft,          ///< Key: Cursor left
  kDown,          ///< Key: Cursor down
  kUp,            ///< Key: Cursor up
  kPageUp,        ///< Key: Page up
  kPageDown,      ///< Key: Page down
  kHome,          ///< Key: Home
  kEnd,           ///< Key: End
  kCapsLock,      ///< Key: Caps lock
  kScrollLock,    ///< Key: Scroll down
  kNumLock,       ///< Key: Num lock
  kPrintScreen,   ///< Key: Print screen
  kPause,         ///< Key: Pause
  kF1,            ///< Key: F1
  kF2,            ///< Key: F2
  kF3,            ///< Key: F3
  kF4,            ///< Key: F4
  kF5,            ///< Key: F5
  kF6,            ///< Key: F6
  kF7,            ///< Key: F7
  kF8,            ///< Key: F8
  kF9,            ///< Key: F9
  kF10,           ///< Key: F10
  kF11,           ///< Key: F11
  kF12,           ///< Key: F12
  kLeftShift,     ///< Key: Shift left
  kLeftControl,   ///< Key: Control left
  kLeftAlt,       ///< Key: Alt left
  kLeftSuper,     ///< Key: Super left
  kRightShift,    ///< Key: Shift right
  kRightControl,  ///< Key: Control right
  kRightAlt,      ///< Key: Alt right
  kRightSuper,    ///< Key: Super right
  kMenu,          ///< Key: KB menu

  // Keypad keys
  kKeypad0,         ///< Key: Keypad 0
  kKeypad1,         ///< Key: Keypad 1
  kKeypad2,         ///< Key: Keypad 2
  kKeypad3,         ///< Key: Keypad 3
  kKeypad4,         ///< Key: Keypad 4
  kKeypad5,         ///< Key: Keypad 5
  kKeypad6,         ///< Key: Keypad 6
  kKeypad7,         ///< Key: Keypad 7
  kKeypad8,         ///< Key: Keypad 8
  kKeypad9,         ///< Key: Keypad 9
  kKeypadDecimal,   ///< Key: Keypad .
  kKeypadDivide,    ///< Key: Keypad /
  kKeypadMultiply,  ///< Key: Keypad *
  kKeypadSubtract,  ///< Key: Keypad -
  kKeypadAdd,       ///< Key: Keypad +
  kKeypadEnter,     ///< Key: Keypad Enter
  kKeypadEqual,     ///< Key: Keypad =

  // Android key buttons
  kAndroidBack,        ///< Key: Android back button
  kAndroidMenu,        ///< Key: Android menu button
  kAndroidVolumeUp,    ///< Key: Android volume up button
  kAndroidVolumeDown,  ///< Key: Android volume down button

  // Meta data
  kEnumKeySize  ///< size of key enum
};

enum class MouseButton : unsigned {
  kLeft,     ///< Mouse button left
  kRight,    ///< Mouse button right
  kMiddle,   ///< Mouse button middle (pressed wheel)
  kSide,     ///< Mouse button side (advanced mouse device)
  kExtra,    ///< Mouse button extra (advanced mouse device)
  kForward,  ///< Mouse button forward (advanced mouse device)
  kBack,     ///< Mouse button back (advanced mouse device)
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
}  // namespace KeyboardMouseInput

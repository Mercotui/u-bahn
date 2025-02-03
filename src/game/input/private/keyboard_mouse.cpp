#include "game/input/keyboard_mouse.h"

#include <absl/log/log.h>

#include <format>
#include <type_traits>
#include <string>

std::string KeyboardMouseInput::KeyName(KeyboardMouseInput::Key key) {
  switch (key) {
    case Key::kApostrophe: {
      return "'";
    }
    case Key::kComma: {
      return ",";
    }
    case Key::kMinus: {
      return "-";
    }
    case Key::kPeriod: {
      return ".";
    }
    case Key::kSlash: {
      return "/";
    }
    case Key::k0: {
      return "0";
    }
    case Key::k1: {
      return "1";
    }
    case Key::k2: {
      return "2";
    }
    case Key::k3: {
      return "3";
    }
    case Key::k4: {
      return "4";
    }
    case Key::k5: {
      return "5";
    }
    case Key::k6: {
      return "6";
    }
    case Key::k7: {
      return "7";
    }
    case Key::k8: {
      return "8";
    }
    case Key::k9: {
      return "9";
    }
    case Key::kSemicolon: {
      return ";";
    }
    case Key::kEqual: {
      return "=";
    }
    case Key::kA: {
      return "A";
    }
    case Key::kB: {
      return "B";
    }
    case Key::kC: {
      return "C";
    }
    case Key::kD: {
      return "D";
    }
    case Key::kE: {
      return "E";
    }
    case Key::kF: {
      return "F";
    }
    case Key::kG: {
      return "G";
    }
    case Key::kH: {
      return "H";
    }
    case Key::kI: {
      return "I";
    }
    case Key::kJ: {
      return "J";
    }
    case Key::kK: {
      return "K";
    }
    case Key::kL: {
      return "L";
    }
    case Key::kM: {
      return "M";
    }
    case Key::kN: {
      return "N";
    }
    case Key::kO: {
      return "O";
    }
    case Key::kP: {
      return "P";
    }
    case Key::kQ: {
      return "Q";
    }
    case Key::kR: {
      return "R";
    }
    case Key::kS: {
      return "S";
    }
    case Key::kT: {
      return "T";
    }
    case Key::kU: {
      return "U";
    }
    case Key::kV: {
      return "V";
    }
    case Key::kW: {
      return "W";
    }
    case Key::kX: {
      return "X";
    }
    case Key::kY: {
      return "Y";
    }
    case Key::kZ: {
      return "Z";
    }
    case Key::kLeftBracket: {
      return "[";
    }
    case Key::kBackslash: {
      return "\\";
    }
    case Key::kRightBracket: {
      return "]";
    }
    case Key::kGrave: {
      return "`";
    }
    case Key::kSpace: {
      return "Space";
    }
    case Key::kEscape: {
      return "Escape";
    }
    case Key::kEnter: {
      return "Enter";
    }
    case Key::kTab: {
      return "Tab";
    }
    case Key::kBackspace: {
      return "Backspace";
    }
    case Key::kInsert: {
      return "Insert";
    }
    case Key::kDelete: {
      return "Delete";
    }
    case Key::kRight: {
      return "Arrow Right";
    }
    case Key::kLeft: {
      return "Arrow Left";
    }
    case Key::kDown: {
      return "Arrow Down";
    }
    case Key::kUp: {
      return "Arrow Up";
    }
    case Key::kPageUp: {
      return "Page Up";
    }
    case Key::kPageDown: {
      return "Page Down";
    }
    case Key::kHome: {
      return "Home";
    }
    case Key::kEnd: {
      return "End";
    }
    case Key::kCapsLock: {
      return "Caps Lock";
    }
    case Key::kScrollLock: {
      return "Scroll Lock";
    }
    case Key::kNumLock: {
      return "Num Lock";
    }
    case Key::kPrintScreen: {
      return "Print Screen";
    }
    case Key::kPause: {
      return "Pause";
    }
    case Key::kF1: {
      return "F1";
    }
    case Key::kF2: {
      return "F2";
    }
    case Key::kF3: {
      return "F3";
    }
    case Key::kF4: {
      return "F4";
    }
    case Key::kF5: {
      return "F5";
    }
    case Key::kF6: {
      return "F6";
    }
    case Key::kF7: {
      return "F7";
    }
    case Key::kF8: {
      return "F8";
    }
    case Key::kF9: {
      return "F9";
    }
    case Key::kF10: {
      return "F10";
    }
    case Key::kF11: {
      return "F11";
    }
    case Key::kF12: {
      return "F12";
    }
    case Key::kLeftShift: {
      return "Left Shift";
    }
    case Key::kLeftControl: {
      return "Left Control";
    }
    case Key::kLeftAlt: {
      return "Left Alt";
    }
    case Key::kLeftSuper: {
      return "Left Super";
    }
    case Key::kRightShift: {
      return "Right Shift";
    }
    case Key::kRightControl: {
      return "Right Control";
    }
    case Key::kRightAlt: {
      return "Right Alt";
    }
    case Key::kRightSuper: {
      return "Right Super";
    }
    case Key::kMenu: {
      return "Menu";
    }
    case Key::kKeypad0: {
      return "Keypad 0";
    }
    case Key::kKeypad1: {
      return "Keypad 1";
    }
    case Key::kKeypad2: {
      return "Keypad 2";
    }
    case Key::kKeypad3: {
      return "Keypad 3";
    }
    case Key::kKeypad4: {
      return "Keypad 4";
    }
    case Key::kKeypad5: {
      return "Keypad 5";
    }
    case Key::kKeypad6: {
      return "Keypad 6";
    }
    case Key::kKeypad7: {
      return "Keypad 7";
    }
    case Key::kKeypad8: {
      return "Keypad 8";
    }
    case Key::kKeypad9: {
      return "Keypad 9";
    }
    case Key::kKeypadDecimal: {
      return "Keypad .";
    }
    case Key::kKeypadDivide: {
      return "Keypad /";
    }
    case Key::kKeypadMultiply: {
      return "Keypad *";
    }
    case Key::kKeypadSubtract: {
      return "Keypad -";
    }
    case Key::kKeypadAdd: {
      return "Keypad +";
    }
    case Key::kKeypadEnter: {
      return "Keypad Enter";
    }
    case Key::kKeypadEqual: {
      return "Keypad =";
    }
    case Key::kAndroidBack: {
      return "Back";
    }
    case Key::kAndroidMenu: {
      return "Menu";
    }
    case Key::kAndroidVolumeUp: {
      return "Volume Up";
    }
    case Key::kAndroidVolumeDown: {
      return "Volume Down";
    }
    default: {
      return std::format("Unknown Key {}", static_cast<std::underlying_type<Key>::type>(key));
    }
  }
}

std::string KeyboardMouseInput::MouseButtonName(KeyboardMouseInput::MouseButton button) {
  switch (button) {
    case MouseButton::kLeft: {
      return "Mouse Button Left";
    }
    case MouseButton::kRight: {
      return "Mouse Button Right";
    }
    case MouseButton::kMiddle: {
      return "Mouse Wheel Button";
    }
    case MouseButton::kSide: {
      return "Mouse Button Side";
    }
    case MouseButton::kExtra: {
      return "Mouse Button Extra";
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

std::string KeyboardMouseInput::MouseAxisName(KeyboardMouseInput::MouseAxis axis) {
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

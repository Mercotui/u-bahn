// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/input/private/raylib_input_helpers.h"

#include <absl/log/log.h>

namespace {
using KeyboardMouseInput::Key;
}  // namespace

Raylib::KeyboardKey RaylibInputHelpers::RaylibKeyboardKey(const Key key) {
  switch (key) {
    case Key::kApostrophe: {
      return Raylib::KEY_APOSTROPHE;
    }
    case Key::kComma: {
      return Raylib::KEY_COMMA;
    }
    case Key::kMinus: {
      return Raylib::KEY_MINUS;
    }
    case Key::kPeriod: {
      return Raylib::KEY_PERIOD;
    }
    case Key::kSlash: {
      return Raylib::KEY_SLASH;
    }
    case Key::k0: {
      return Raylib::KEY_ZERO;
    }
    case Key::k1: {
      return Raylib::KEY_ONE;
    }
    case Key::k2: {
      return Raylib::KEY_TWO;
    }
    case Key::k3: {
      return Raylib::KEY_THREE;
    }
    case Key::k4: {
      return Raylib::KEY_FOUR;
    }
    case Key::k5: {
      return Raylib::KEY_FIVE;
    }
    case Key::k6: {
      return Raylib::KEY_SIX;
    }
    case Key::k7: {
      return Raylib::KEY_SEVEN;
    }
    case Key::k8: {
      return Raylib::KEY_EIGHT;
    }
    case Key::k9: {
      return Raylib::KEY_NINE;
    }
    case Key::kSemicolon: {
      return Raylib::KEY_SEMICOLON;
    }
    case Key::kEqual: {
      return Raylib::KEY_EQUAL;
    }
    case Key::kA: {
      return Raylib::KEY_A;
    }
    case Key::kB: {
      return Raylib::KEY_B;
    }
    case Key::kC: {
      return Raylib::KEY_C;
    }
    case Key::kD: {
      return Raylib::KEY_D;
    }
    case Key::kE: {
      return Raylib::KEY_E;
    }
    case Key::kF: {
      return Raylib::KEY_F;
    }
    case Key::kG: {
      return Raylib::KEY_G;
    }
    case Key::kH: {
      return Raylib::KEY_H;
    }
    case Key::kI: {
      return Raylib::KEY_I;
    }
    case Key::kJ: {
      return Raylib::KEY_J;
    }
    case Key::kK: {
      return Raylib::KEY_K;
    }
    case Key::kL: {
      return Raylib::KEY_L;
    }
    case Key::kM: {
      return Raylib::KEY_M;
    }
    case Key::kN: {
      return Raylib::KEY_N;
    }
    case Key::kO: {
      return Raylib::KEY_O;
    }
    case Key::kP: {
      return Raylib::KEY_P;
    }
    case Key::kQ: {
      return Raylib::KEY_Q;
    }
    case Key::kR: {
      return Raylib::KEY_R;
    }
    case Key::kS: {
      return Raylib::KEY_S;
    }
    case Key::kT: {
      return Raylib::KEY_T;
    }
    case Key::kU: {
      return Raylib::KEY_U;
    }
    case Key::kV: {
      return Raylib::KEY_V;
    }
    case Key::kW: {
      return Raylib::KEY_W;
    }
    case Key::kX: {
      return Raylib::KEY_X;
    }
    case Key::kY: {
      return Raylib::KEY_Y;
    }
    case Key::kZ: {
      return Raylib::KEY_Z;
    }
    case Key::kLeftBracket: {
      return Raylib::KEY_LEFT_BRACKET;
    }
    case Key::kBackslash: {
      return Raylib::KEY_BACKSLASH;
    }
    case Key::kRightBracket: {
      return Raylib::KEY_RIGHT_BRACKET;
    }
    case Key::kGrave: {
      return Raylib::KEY_GRAVE;
    }
    case Key::kSpace: {
      return Raylib::KEY_SPACE;
    }
    case Key::kEscape: {
      return Raylib::KEY_ESCAPE;
    }
    case Key::kEnter: {
      return Raylib::KEY_ENTER;
    }
    case Key::kTab: {
      return Raylib::KEY_TAB;
    }
    case Key::kBackspace: {
      return Raylib::KEY_BACKSPACE;
    }
    case Key::kInsert: {
      return Raylib::KEY_INSERT;
    }
    case Key::kDelete: {
      return Raylib::KEY_DELETE;
    }
    case Key::kRight: {
      return Raylib::KEY_RIGHT;
    }
    case Key::kLeft: {
      return Raylib::KEY_LEFT;
    }
    case Key::kDown: {
      return Raylib::KEY_DOWN;
    }
    case Key::kUp: {
      return Raylib::KEY_UP;
    }
    case Key::kPageUp: {
      return Raylib::KEY_PAGE_UP;
    }
    case Key::kPageDown: {
      return Raylib::KEY_PAGE_DOWN;
    }
    case Key::kHome: {
      return Raylib::KEY_HOME;
    }
    case Key::kEnd: {
      return Raylib::KEY_END;
    }
    case Key::kCapsLock: {
      return Raylib::KEY_CAPS_LOCK;
    }
    case Key::kScrollLock: {
      return Raylib::KEY_SCROLL_LOCK;
    }
    case Key::kNumLock: {
      return Raylib::KEY_NUM_LOCK;
    }
    case Key::kPrintScreen: {
      return Raylib::KEY_PRINT_SCREEN;
    }
    case Key::kPause: {
      return Raylib::KEY_PAUSE;
    }
    case Key::kF1: {
      return Raylib::KEY_F1;
    }
    case Key::kF2: {
      return Raylib::KEY_F2;
    }
    case Key::kF3: {
      return Raylib::KEY_F3;
    }
    case Key::kF4: {
      return Raylib::KEY_F4;
    }
    case Key::kF5: {
      return Raylib::KEY_F5;
    }
    case Key::kF6: {
      return Raylib::KEY_F6;
    }
    case Key::kF7: {
      return Raylib::KEY_F7;
    }
    case Key::kF8: {
      return Raylib::KEY_F8;
    }
    case Key::kF9: {
      return Raylib::KEY_F9;
    }
    case Key::kF10: {
      return Raylib::KEY_F10;
    }
    case Key::kF11: {
      return Raylib::KEY_F11;
    }
    case Key::kF12: {
      return Raylib::KEY_F12;
    }
    case Key::kLeftShift: {
      return Raylib::KEY_LEFT_SHIFT;
    }
    case Key::kLeftControl: {
      return Raylib::KEY_LEFT_CONTROL;
    }
    case Key::kLeftAlt: {
      return Raylib::KEY_LEFT_ALT;
    }
    case Key::kLeftSuper: {
      return Raylib::KEY_LEFT_SUPER;
    }
    case Key::kRightShift: {
      return Raylib::KEY_RIGHT_SHIFT;
    }
    case Key::kRightControl: {
      return Raylib::KEY_RIGHT_CONTROL;
    }
    case Key::kRightAlt: {
      return Raylib::KEY_RIGHT_ALT;
    }
    case Key::kRightSuper: {
      return Raylib::KEY_RIGHT_SUPER;
    }
    case Key::kMenu: {
      return Raylib::KEY_MENU;
    }
    case Key::kKeypad0: {
      return Raylib::KEY_KP_0;
    }
    case Key::kKeypad1: {
      return Raylib::KEY_KP_1;
    }
    case Key::kKeypad2: {
      return Raylib::KEY_KP_2;
    }
    case Key::kKeypad3: {
      return Raylib::KEY_KP_3;
    }
    case Key::kKeypad4: {
      return Raylib::KEY_KP_4;
    }
    case Key::kKeypad5: {
      return Raylib::KEY_KP_5;
    }
    case Key::kKeypad6: {
      return Raylib::KEY_KP_6;
    }
    case Key::kKeypad7: {
      return Raylib::KEY_KP_7;
    }
    case Key::kKeypad8: {
      return Raylib::KEY_KP_8;
    }
    case Key::kKeypad9: {
      return Raylib::KEY_KP_9;
    }
    case Key::kKeypadDecimal: {
      return Raylib::KEY_KP_DECIMAL;
    }
    case Key::kKeypadDivide: {
      return Raylib::KEY_KP_DIVIDE;
    }
    case Key::kKeypadMultiply: {
      return Raylib::KEY_KP_MULTIPLY;
    }
    case Key::kKeypadSubtract: {
      return Raylib::KEY_KP_SUBTRACT;
    }
    case Key::kKeypadAdd: {
      return Raylib::KEY_KP_ADD;
    }
    case Key::kKeypadEnter: {
      return Raylib::KEY_KP_ENTER;
    }
    case Key::kKeypadEqual: {
      return Raylib::KEY_KP_EQUAL;
    }
    case Key::kAndroidBack: {
      return Raylib::KEY_BACK;
    }
    case Key::kAndroidMenu: {
      return Raylib::KEY_MENU;
    }
    case Key::kAndroidVolumeUp: {
      return Raylib::KEY_VOLUME_UP;
    }
    case Key::kAndroidVolumeDown: {
      return Raylib::KEY_VOLUME_DOWN;
    }
    default: {
      return Raylib::KEY_NULL;
    }
  }
}

std::optional<Raylib::MouseButton> RaylibInputHelpers::RaylibMouseButton(KeyboardMouseInput::MouseButton button) {
  switch (button) {
    case KeyboardMouseInput::MouseButton::kLeft: {
      return Raylib::MOUSE_BUTTON_LEFT;
    }
    case KeyboardMouseInput::MouseButton::kRight: {
      return Raylib::MOUSE_BUTTON_RIGHT;
    }
    case KeyboardMouseInput::MouseButton::kMiddle: {
      return Raylib::MOUSE_BUTTON_MIDDLE;
    }
    case KeyboardMouseInput::MouseButton::kSide: {
      return Raylib::MOUSE_BUTTON_SIDE;
    }
    case KeyboardMouseInput::MouseButton::kExtra: {
      return Raylib::MOUSE_BUTTON_EXTRA;
    }
    case KeyboardMouseInput::MouseButton::kForward: {
      return Raylib::MOUSE_BUTTON_FORWARD;
    }
    case KeyboardMouseInput::MouseButton::kBack: {
      return Raylib::MOUSE_BUTTON_BACK;
    }
    default: {
      return std::nullopt;
    }
  }
}

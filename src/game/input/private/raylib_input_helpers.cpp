#include "game/input/private/raylib_input_helpers.h"

#include <absl/log/log.h>

#include <type_traits>

namespace {
using KeyboardMouseInput::Key;
}  // namespace

KeyboardKey RaylibInputHelpers::RaylibKeyboardKey(KeyboardMouseInput::Key key) {
  switch (key) {
    case Key::kApostrophe: {
      return KEY_APOSTROPHE;
    }
    case Key::kComma: {
      return KEY_COMMA;
    }
    case Key::kMinus: {
      return KEY_MINUS;
    }
    case Key::kPeriod: {
      return KEY_PERIOD;
    }
    case Key::kSlash: {
      return KEY_SLASH;
    }
    case Key::k0: {
      return KEY_ZERO;
    }
    case Key::k1: {
      return KEY_ONE;
    }
    case Key::k2: {
      return KEY_TWO;
    }
    case Key::k3: {
      return KEY_THREE;
    }
    case Key::k4: {
      return KEY_FOUR;
    }
    case Key::k5: {
      return KEY_FIVE;
    }
    case Key::k6: {
      return KEY_SIX;
    }
    case Key::k7: {
      return KEY_SEVEN;
    }
    case Key::k8: {
      return KEY_EIGHT;
    }
    case Key::k9: {
      return KEY_NINE;
    }
    case Key::kSemicolon: {
      return KEY_SEMICOLON;
    }
    case Key::kEqual: {
      return KEY_EQUAL;
    }
    case Key::kA: {
      return KEY_A;
    }
    case Key::kB: {
      return KEY_B;
    }
    case Key::kC: {
      return KEY_C;
    }
    case Key::kD: {
      return KEY_D;
    }
    case Key::kE: {
      return KEY_E;
    }
    case Key::kF: {
      return KEY_F;
    }
    case Key::kG: {
      return KEY_G;
    }
    case Key::kH: {
      return KEY_H;
    }
    case Key::kI: {
      return KEY_I;
    }
    case Key::kJ: {
      return KEY_J;
    }
    case Key::kK: {
      return KEY_K;
    }
    case Key::kL: {
      return KEY_L;
    }
    case Key::kM: {
      return KEY_M;
    }
    case Key::kN: {
      return KEY_N;
    }
    case Key::kO: {
      return KEY_O;
    }
    case Key::kP: {
      return KEY_P;
    }
    case Key::kQ: {
      return KEY_Q;
    }
    case Key::kR: {
      return KEY_R;
    }
    case Key::kS: {
      return KEY_S;
    }
    case Key::kT: {
      return KEY_T;
    }
    case Key::kU: {
      return KEY_U;
    }
    case Key::kV: {
      return KEY_V;
    }
    case Key::kW: {
      return KEY_W;
    }
    case Key::kX: {
      return KEY_X;
    }
    case Key::kY: {
      return KEY_Y;
    }
    case Key::kZ: {
      return KEY_Z;
    }
    case Key::kLeftBracket: {
      return KEY_LEFT_BRACKET;
    }
    case Key::kBackslash: {
      return KEY_BACKSLASH;
    }
    case Key::kRightBracket: {
      return KEY_RIGHT_BRACKET;
    }
    case Key::kGrave: {
      return KEY_GRAVE;
    }
    case Key::kSpace: {
      return KEY_SPACE;
    }
    case Key::kEscape: {
      return KEY_ESCAPE;
    }
    case Key::kEnter: {
      return KEY_ENTER;
    }
    case Key::kTab: {
      return KEY_TAB;
    }
    case Key::kBackspace: {
      return KEY_BACKSPACE;
    }
    case Key::kInsert: {
      return KEY_INSERT;
    }
    case Key::kDelete: {
      return KEY_DELETE;
    }
    case Key::kRight: {
      return KEY_RIGHT;
    }
    case Key::kLeft: {
      return KEY_LEFT;
    }
    case Key::kDown: {
      return KEY_DOWN;
    }
    case Key::kUp: {
      return KEY_UP;
    }
    case Key::kPageUp: {
      return KEY_PAGE_UP;
    }
    case Key::kPageDown: {
      return KEY_PAGE_DOWN;
    }
    case Key::kHome: {
      return KEY_HOME;
    }
    case Key::kEnd: {
      return KEY_END;
    }
    case Key::kCapsLock: {
      return KEY_CAPS_LOCK;
    }
    case Key::kScrollLock: {
      return KEY_SCROLL_LOCK;
    }
    case Key::kNumLock: {
      return KEY_NUM_LOCK;
    }
    case Key::kPrintScreen: {
      return KEY_PRINT_SCREEN;
    }
    case Key::kPause: {
      return KEY_PAUSE;
    }
    case Key::kF1: {
      return KEY_F1;
    }
    case Key::kF2: {
      return KEY_F2;
    }
    case Key::kF3: {
      return KEY_F3;
    }
    case Key::kF4: {
      return KEY_F4;
    }
    case Key::kF5: {
      return KEY_F5;
    }
    case Key::kF6: {
      return KEY_F6;
    }
    case Key::kF7: {
      return KEY_F7;
    }
    case Key::kF8: {
      return KEY_F8;
    }
    case Key::kF9: {
      return KEY_F9;
    }
    case Key::kF10: {
      return KEY_F10;
    }
    case Key::kF11: {
      return KEY_F11;
    }
    case Key::kF12: {
      return KEY_F12;
    }
    case Key::kLeftShift: {
      return KEY_LEFT_SHIFT;
    }
    case Key::kLeftControl: {
      return KEY_LEFT_CONTROL;
    }
    case Key::kLeftAlt: {
      return KEY_LEFT_ALT;
    }
    case Key::kLeftSuper: {
      return KEY_LEFT_SUPER;
    }
    case Key::kRightShift: {
      return KEY_RIGHT_SHIFT;
    }
    case Key::kRightControl: {
      return KEY_RIGHT_CONTROL;
    }
    case Key::kRightAlt: {
      return KEY_RIGHT_ALT;
    }
    case Key::kRightSuper: {
      return KEY_RIGHT_SUPER;
    }
    case Key::kMenu: {
      return KEY_MENU;
    }
    case Key::kKeypad0: {
      return KEY_KP_0;
    }
    case Key::kKeypad1: {
      return KEY_KP_1;
    }
    case Key::kKeypad2: {
      return KEY_KP_2;
    }
    case Key::kKeypad3: {
      return KEY_KP_3;
    }
    case Key::kKeypad4: {
      return KEY_KP_4;
    }
    case Key::kKeypad5: {
      return KEY_KP_5;
    }
    case Key::kKeypad6: {
      return KEY_KP_6;
    }
    case Key::kKeypad7: {
      return KEY_KP_7;
    }
    case Key::kKeypad8: {
      return KEY_KP_8;
    }
    case Key::kKeypad9: {
      return KEY_KP_9;
    }
    case Key::kKeypadDecimal: {
      return KEY_KP_DECIMAL;
    }
    case Key::kKeypadDivide: {
      return KEY_KP_DIVIDE;
    }
    case Key::kKeypadMultiply: {
      return KEY_KP_MULTIPLY;
    }
    case Key::kKeypadSubtract: {
      return KEY_KP_SUBTRACT;
    }
    case Key::kKeypadAdd: {
      return KEY_KP_ADD;
    }
    case Key::kKeypadEnter: {
      return KEY_KP_ENTER;
    }
    case Key::kKeypadEqual: {
      return KEY_KP_EQUAL;
    }
    case Key::kAndroidBack: {
      return KEY_BACK;
    }
    case Key::kAndroidMenu: {
      return KEY_MENU;
    }
    case Key::kAndroidVolumeUp: {
      return KEY_VOLUME_UP;
    }
    case Key::kAndroidVolumeDown: {
      return KEY_VOLUME_DOWN;
    }
    default: {
      return KEY_NULL;
    }
  }
}

MouseButton RaylibInputHelpers::RaylibMouseButton(KeyboardMouseInput::MouseButton button) {
  switch (button) {
    case KeyboardMouseInput::MouseButton::kLeft: {
      return MOUSE_BUTTON_LEFT;
    }
    case KeyboardMouseInput::MouseButton::kRight: {
      return MOUSE_BUTTON_RIGHT;
    }
    case KeyboardMouseInput::MouseButton::kMiddle: {
      return MOUSE_BUTTON_MIDDLE;
    }
    case KeyboardMouseInput::MouseButton::kSide: {
      return MOUSE_BUTTON_SIDE;
    }
    case KeyboardMouseInput::MouseButton::kExtra: {
      return MOUSE_BUTTON_EXTRA;
    }
    case KeyboardMouseInput::MouseButton::kForward: {
      return MOUSE_BUTTON_FORWARD;
    }
    case KeyboardMouseInput::MouseButton::kBack: {
      return MOUSE_BUTTON_BACK;
    }
    default: {
      LOG(FATAL) << "RaylibMouseButton trying to convert unknown value button"
                 << static_cast<std::underlying_type<MouseButton>::type>(button);
      return MOUSE_BUTTON_LEFT;
    }
  }
}

// SPDX-FileCopyrightText: 2026 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#include "platform/private/input_helpers.h"

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <absl/log/log.h>

#include <chrono>
#include <cmath>
#include <limits>
#include <string>
#include <utility>

#include "platform/keyboard_mouse.h"

namespace {
using std::chrono_literals::operator""s;
using Sample = Input::Axis::Sample;

//! Defined in change of units per ms
constexpr float kActivityThreshold = 0.0006f;
constexpr bool kActivityDetectionLogging{false};

float EstimateDerivative(const Sample& old_sample, const Sample& new_sample) {
  const std::chrono::duration<float, std::milli> time_delta = new_sample.time_point - old_sample.time_point;
  const float divider = time_delta.count();
  if (!std::isnormal(divider)) {
    return NAN;
  }
  return (new_sample.value - old_sample.value) / divider;
}

}  // namespace

KeyboardMouseInput::Key InputSdlHelpers::ToKey(const SDL_Scancode scancode) {
  using KeyboardMouseInput::Key;
  switch (scancode) {
    case SDL_SCANCODE_1: return Key::k1;
    case SDL_SCANCODE_2: return Key::k2;
    case SDL_SCANCODE_3: return Key::k3;
    case SDL_SCANCODE_4: return Key::k4;
    case SDL_SCANCODE_5: return Key::k5;
    case SDL_SCANCODE_6: return Key::k6;
    case SDL_SCANCODE_7: return Key::k7;
    case SDL_SCANCODE_8: return Key::k8;
    case SDL_SCANCODE_9: return Key::k9;
    case SDL_SCANCODE_0: return Key::k0;
    case SDL_SCANCODE_A: return Key::kA;
    case SDL_SCANCODE_B: return Key::kB;
    case SDL_SCANCODE_C: return Key::kC;
    case SDL_SCANCODE_D: return Key::kD;
    case SDL_SCANCODE_E: return Key::kE;
    case SDL_SCANCODE_F: return Key::kF;
    case SDL_SCANCODE_G: return Key::kG;
    case SDL_SCANCODE_H: return Key::kH;
    case SDL_SCANCODE_I: return Key::kI;
    case SDL_SCANCODE_J: return Key::kJ;
    case SDL_SCANCODE_K: return Key::kK;
    case SDL_SCANCODE_L: return Key::kL;
    case SDL_SCANCODE_M: return Key::kM;
    case SDL_SCANCODE_N: return Key::kN;
    case SDL_SCANCODE_O: return Key::kO;
    case SDL_SCANCODE_P: return Key::kP;
    case SDL_SCANCODE_Q: return Key::kQ;
    case SDL_SCANCODE_R: return Key::kR;
    case SDL_SCANCODE_S: return Key::kS;
    case SDL_SCANCODE_T: return Key::kT;
    case SDL_SCANCODE_U: return Key::kU;
    case SDL_SCANCODE_V: return Key::kV;
    case SDL_SCANCODE_W: return Key::kW;
    case SDL_SCANCODE_X: return Key::kX;
    case SDL_SCANCODE_Y: return Key::kY;
    case SDL_SCANCODE_Z: return Key::kZ;
    case SDL_SCANCODE_RETURN: return Key::kEnter;
    case SDL_SCANCODE_ESCAPE: return Key::kEscape;
    case SDL_SCANCODE_BACKSPACE: return Key::kBackspace;
    case SDL_SCANCODE_TAB: return Key::kTab;
    case SDL_SCANCODE_SPACE: return Key::kSpace;
    case SDL_SCANCODE_MINUS: return Key::kMinus;
    case SDL_SCANCODE_EQUALS: return Key::kEqual;
    case SDL_SCANCODE_LEFTBRACKET: return Key::kLeftBracket;
    case SDL_SCANCODE_RIGHTBRACKET: return Key::kRightBracket;
    case SDL_SCANCODE_BACKSLASH: return Key::kBackslash;
    case SDL_SCANCODE_SEMICOLON: return Key::kSemicolon;
    case SDL_SCANCODE_APOSTROPHE: return Key::kApostrophe;
    case SDL_SCANCODE_GRAVE: return Key::kGrave;
    case SDL_SCANCODE_COMMA: return Key::kComma;
    case SDL_SCANCODE_PERIOD: return Key::kPeriod;
    case SDL_SCANCODE_SLASH: return Key::kSlash;
    case SDL_SCANCODE_CAPSLOCK: return Key::kCapsLock;
    case SDL_SCANCODE_F1: return Key::kF1;
    case SDL_SCANCODE_F2: return Key::kF2;
    case SDL_SCANCODE_F3: return Key::kF3;
    case SDL_SCANCODE_F4: return Key::kF4;
    case SDL_SCANCODE_F5: return Key::kF5;
    case SDL_SCANCODE_F6: return Key::kF6;
    case SDL_SCANCODE_F7: return Key::kF7;
    case SDL_SCANCODE_F8: return Key::kF8;
    case SDL_SCANCODE_F9: return Key::kF9;
    case SDL_SCANCODE_F10: return Key::kF10;
    case SDL_SCANCODE_F11: return Key::kF11;
    case SDL_SCANCODE_F12: return Key::kF12;
    case SDL_SCANCODE_PRINTSCREEN: return Key::kPrintScreen;
    case SDL_SCANCODE_SCROLLLOCK: return Key::kScrollLock;
    case SDL_SCANCODE_PAUSE: return Key::kPause;
    case SDL_SCANCODE_INSERT: return Key::kInsert;
    case SDL_SCANCODE_HOME: return Key::kHome;
    case SDL_SCANCODE_PAGEUP: return Key::kPageUp;
    case SDL_SCANCODE_DELETE: return Key::kDelete;
    case SDL_SCANCODE_END: return Key::kEnd;
    case SDL_SCANCODE_PAGEDOWN: return Key::kPageDown;
    case SDL_SCANCODE_RIGHT: return Key::kRight;
    case SDL_SCANCODE_LEFT: return Key::kLeft;
    case SDL_SCANCODE_DOWN: return Key::kDown;
    case SDL_SCANCODE_UP: return Key::kUp;
    case SDL_SCANCODE_NUMLOCKCLEAR: return Key::kNumLock;
    case SDL_SCANCODE_KP_DIVIDE: return Key::kKeypadDivide;
    case SDL_SCANCODE_KP_MULTIPLY: return Key::kKeypadMultiply;
    case SDL_SCANCODE_KP_MINUS: return Key::kKeypadSubtract;
    case SDL_SCANCODE_KP_PLUS: return Key::kKeypadAdd;
    case SDL_SCANCODE_KP_ENTER: return Key::kKeypadEnter;
    case SDL_SCANCODE_KP_EQUALS: return Key::kKeypadEqual;
    case SDL_SCANCODE_KP_DECIMAL: return Key::kKeypadDecimal;
    case SDL_SCANCODE_KP_1: return Key::kKeypad1;
    case SDL_SCANCODE_KP_2: return Key::kKeypad2;
    case SDL_SCANCODE_KP_3: return Key::kKeypad3;
    case SDL_SCANCODE_KP_4: return Key::kKeypad4;
    case SDL_SCANCODE_KP_5: return Key::kKeypad5;
    case SDL_SCANCODE_KP_6: return Key::kKeypad6;
    case SDL_SCANCODE_KP_7: return Key::kKeypad7;
    case SDL_SCANCODE_KP_8: return Key::kKeypad8;
    case SDL_SCANCODE_KP_9: return Key::kKeypad9;
    case SDL_SCANCODE_KP_0: return Key::kKeypad0;
    case SDL_SCANCODE_LSHIFT: return Key::kLeftShift;
    case SDL_SCANCODE_LCTRL: return Key::kLeftControl;
    case SDL_SCANCODE_LALT: return Key::kLeftAlt;
    case SDL_SCANCODE_LGUI: return Key::kLeftSuper;
    case SDL_SCANCODE_RSHIFT: return Key::kRightShift;
    case SDL_SCANCODE_RCTRL: return Key::kRightControl;
    case SDL_SCANCODE_RALT: return Key::kRightAlt;
    case SDL_SCANCODE_RGUI: return Key::kRightSuper;
    case SDL_SCANCODE_MENU: return Key::kMenu;
    default: return Key::kUnspecified;
  }
}

SDL_Scancode InputSdlHelpers::FromKey(const KeyboardMouseInput::Key key) {
  using KeyboardMouseInput::Key;
  switch (key) {
    case Key::k1: return SDL_SCANCODE_1;
    case Key::k2: return SDL_SCANCODE_2;
    case Key::k3: return SDL_SCANCODE_3;
    case Key::k4: return SDL_SCANCODE_4;
    case Key::k5: return SDL_SCANCODE_5;
    case Key::k6: return SDL_SCANCODE_6;
    case Key::k7: return SDL_SCANCODE_7;
    case Key::k8: return SDL_SCANCODE_8;
    case Key::k9: return SDL_SCANCODE_9;
    case Key::k0: return SDL_SCANCODE_0;
    case Key::kA: return SDL_SCANCODE_A;
    case Key::kB: return SDL_SCANCODE_B;
    case Key::kC: return SDL_SCANCODE_C;
    case Key::kD: return SDL_SCANCODE_D;
    case Key::kE: return SDL_SCANCODE_E;
    case Key::kF: return SDL_SCANCODE_F;
    case Key::kG: return SDL_SCANCODE_G;
    case Key::kH: return SDL_SCANCODE_H;
    case Key::kI: return SDL_SCANCODE_I;
    case Key::kJ: return SDL_SCANCODE_J;
    case Key::kK: return SDL_SCANCODE_K;
    case Key::kL: return SDL_SCANCODE_L;
    case Key::kM: return SDL_SCANCODE_M;
    case Key::kN: return SDL_SCANCODE_N;
    case Key::kO: return SDL_SCANCODE_O;
    case Key::kP: return SDL_SCANCODE_P;
    case Key::kQ: return SDL_SCANCODE_Q;
    case Key::kR: return SDL_SCANCODE_R;
    case Key::kS: return SDL_SCANCODE_S;
    case Key::kT: return SDL_SCANCODE_T;
    case Key::kU: return SDL_SCANCODE_U;
    case Key::kV: return SDL_SCANCODE_V;
    case Key::kW: return SDL_SCANCODE_W;
    case Key::kX: return SDL_SCANCODE_X;
    case Key::kY: return SDL_SCANCODE_Y;
    case Key::kZ: return SDL_SCANCODE_Z;
    case Key::kEnter: return SDL_SCANCODE_RETURN;
    case Key::kEscape: return SDL_SCANCODE_ESCAPE;
    case Key::kBackspace: return SDL_SCANCODE_BACKSPACE;
    case Key::kTab: return SDL_SCANCODE_TAB;
    case Key::kSpace: return SDL_SCANCODE_SPACE;
    case Key::kMinus: return SDL_SCANCODE_MINUS;
    case Key::kEqual: return SDL_SCANCODE_EQUALS;
    case Key::kLeftBracket: return SDL_SCANCODE_LEFTBRACKET;
    case Key::kRightBracket: return SDL_SCANCODE_RIGHTBRACKET;
    case Key::kBackslash: return SDL_SCANCODE_BACKSLASH;
    case Key::kSemicolon: return SDL_SCANCODE_SEMICOLON;
    case Key::kApostrophe: return SDL_SCANCODE_APOSTROPHE;
    case Key::kGrave: return SDL_SCANCODE_GRAVE;
    case Key::kComma: return SDL_SCANCODE_COMMA;
    case Key::kPeriod: return SDL_SCANCODE_PERIOD;
    case Key::kSlash: return SDL_SCANCODE_SLASH;
    case Key::kCapsLock: return SDL_SCANCODE_CAPSLOCK;
    case Key::kF1: return SDL_SCANCODE_F1;
    case Key::kF2: return SDL_SCANCODE_F2;
    case Key::kF3: return SDL_SCANCODE_F3;
    case Key::kF4: return SDL_SCANCODE_F4;
    case Key::kF5: return SDL_SCANCODE_F5;
    case Key::kF6: return SDL_SCANCODE_F6;
    case Key::kF7: return SDL_SCANCODE_F7;
    case Key::kF8: return SDL_SCANCODE_F8;
    case Key::kF9: return SDL_SCANCODE_F9;
    case Key::kF10: return SDL_SCANCODE_F10;
    case Key::kF11: return SDL_SCANCODE_F11;
    case Key::kF12: return SDL_SCANCODE_F12;
    case Key::kPrintScreen: return SDL_SCANCODE_PRINTSCREEN;
    case Key::kScrollLock: return SDL_SCANCODE_SCROLLLOCK;
    case Key::kPause: return SDL_SCANCODE_PAUSE;
    case Key::kInsert: return SDL_SCANCODE_INSERT;
    case Key::kHome: return SDL_SCANCODE_HOME;
    case Key::kPageUp: return SDL_SCANCODE_PAGEUP;
    case Key::kDelete: return SDL_SCANCODE_DELETE;
    case Key::kEnd: return SDL_SCANCODE_END;
    case Key::kPageDown: return SDL_SCANCODE_PAGEDOWN;
    case Key::kRight: return SDL_SCANCODE_RIGHT;
    case Key::kLeft: return SDL_SCANCODE_LEFT;
    case Key::kDown: return SDL_SCANCODE_DOWN;
    case Key::kUp: return SDL_SCANCODE_UP;
    case Key::kNumLock: return SDL_SCANCODE_NUMLOCKCLEAR;
    case Key::kKeypadDivide: return SDL_SCANCODE_KP_DIVIDE;
    case Key::kKeypadMultiply: return SDL_SCANCODE_KP_MULTIPLY;
    case Key::kKeypadSubtract: return SDL_SCANCODE_KP_MINUS;
    case Key::kKeypadAdd: return SDL_SCANCODE_KP_PLUS;
    case Key::kKeypadEnter: return SDL_SCANCODE_KP_ENTER;
    case Key::kKeypadEqual: return SDL_SCANCODE_KP_EQUALS;
    case Key::kKeypadDecimal: return SDL_SCANCODE_KP_DECIMAL;
    case Key::kKeypad1: return SDL_SCANCODE_KP_1;
    case Key::kKeypad2: return SDL_SCANCODE_KP_2;
    case Key::kKeypad3: return SDL_SCANCODE_KP_3;
    case Key::kKeypad4: return SDL_SCANCODE_KP_4;
    case Key::kKeypad5: return SDL_SCANCODE_KP_5;
    case Key::kKeypad6: return SDL_SCANCODE_KP_6;
    case Key::kKeypad7: return SDL_SCANCODE_KP_7;
    case Key::kKeypad8: return SDL_SCANCODE_KP_8;
    case Key::kKeypad9: return SDL_SCANCODE_KP_9;
    case Key::kKeypad0: return SDL_SCANCODE_KP_0;
    case Key::kLeftShift: return SDL_SCANCODE_LSHIFT;
    case Key::kLeftControl: return SDL_SCANCODE_LCTRL;
    case Key::kLeftAlt: return SDL_SCANCODE_LALT;
    case Key::kLeftSuper: return SDL_SCANCODE_LGUI;
    case Key::kRightShift: return SDL_SCANCODE_RSHIFT;
    case Key::kRightControl: return SDL_SCANCODE_RCTRL;
    case Key::kRightAlt: return SDL_SCANCODE_RALT;
    case Key::kRightSuper: return SDL_SCANCODE_RGUI;
    case Key::kMenu: return SDL_SCANCODE_MENU;
    default: return SDL_SCANCODE_UNKNOWN;
  }
}

KeyboardMouseInput::MouseButton InputSdlHelpers::ToMouseButton(const SDL_MouseButtonFlags button) {
  using KeyboardMouseInput::MouseButton;
  switch (button) {
    case SDL_BUTTON_LEFT: return MouseButton::kLeft;
    case SDL_BUTTON_MIDDLE: return MouseButton::kMiddle;
    case SDL_BUTTON_RIGHT: return MouseButton::kRight;
    case SDL_BUTTON_X1: return MouseButton::kForward;
    case SDL_BUTTON_X2: return MouseButton::kBack;
    default: return MouseButton::kUnspecified;
  }
}

bool InputAxisHelpers::Assign(Input::Axis* axis, const Input::Axis::Sample new_sample) {
  axis->value = new_sample.value;
  auto& history = axis->history;
  axis->history_size = axis->history_size + 1 % std::numeric_limits<decltype(axis->history_size)>::max();

  if (history.second.time_point == new_sample.time_point) {
    // if multiple samples are recorded for the same time point, we only keep the newest value.
    history.second.value = new_sample.value;
  } else {
    history.first = history.second;
    history.second = new_sample;
  }

  // Don't attempt activity detection on uninitialized values
  if (axis->history_size < 2) {
    return false;
  }

  if ((history.second.time_point - history.first.time_point) > 1.0s) {
    // we ignore samples older than 1 second
    return false;
  }

  const auto derivative = EstimateDerivative(history.first, history.second);
  if (!std::isnormal(derivative)) {
    return false;
  }
  const bool is_active = kActivityThreshold < fabsf(derivative);

  if constexpr (kActivityDetectionLogging) {
    LOG(INFO) << "Activity detection for" << "," << new_sample.value << "," << derivative << ","
              << (is_active ? "1.0" : "0.0");
  }
  return is_active;
}

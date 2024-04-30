#include "joystick_handler.h"

#include <iostream>

#include "game/input/input.h"
#include "libenjoy.h"

using JoyAxis = Joystick::JoyAxis;

namespace {
JoyAxis RemapAxisRange(float value) {
  // low2 + (value - low1) * (high2 - low2) / (high1 - low1)
  return JoyAxis{-5.0f + (value + 32767.0f) * 10.0f / (32767.0f * 2.0f)};
}
}  // namespace

Joystick::Joystick(int id) {
  // auto joy_stick = libenjoy_open_joystick(joy_context, joy_id);
  //  int libenjoy_get_axes_num(libenjoy_joystick *joy);
  //  int libenjoy_get_buttons_num(libenjoy_joystick *joy);
}

JoystickHandler::JoystickHandler() : joy_context_(libenjoy_init()) {}
JoystickHandler::~JoystickHandler() { libenjoy_close(joy_context_); }

std::vector<Input::Info> JoystickHandler::ListInputs() const {
  // TODO(Menno 30.04.2024) This should probably happen on construction and on LIBENJOY_EV_CONNECTED
  std::vector<Input::Info> joysticks;

  libenjoy_enumerate(joy_context_);
  auto list = libenjoy_get_info_list(joy_context_);

  for (int i = 0; i < list->count; i++) {
    auto entry = list->list[i];
    joysticks.push_back(Input::Info({
        .id = static_cast<int>(entry->id),
        .name = entry->name,
        .type = Input::Type::kJoystick,
    }));
  }

  libenjoy_free_info_list(list);
  return joysticks;
}

void JoystickHandler::Poll() {
  libenjoy_event event;
  while (libenjoy_poll(joy_context_, &event)) {
    switch (event.type) {
      case LIBENJOY_EV_AXIS:
        printf("%u: axis %d val %d\n", event.joy_id, event.part_id, event.data);
        break;
      case LIBENJOY_EV_BUTTON:
        printf("%u: button %d val %d\n", event.joy_id, event.part_id, event.data);
        break;
      case LIBENJOY_EV_CONNECTED:
        printf("%u: status changed: %d\n", event.joy_id, event.data);
        break;
    }
  }
}

void JoystickHandler::SetConfig(int id, Input::Config) {}

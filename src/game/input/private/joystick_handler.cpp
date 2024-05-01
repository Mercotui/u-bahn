#include "joystick_handler.h"

#include <absl/log/log.h>

#include <iostream>

#include "game/input/input.h"
#include "libenjoy.h"

namespace {
float RemapAxisRange(float value) {
  // low2 + (pressed - low1) * (high2 - low2) / (high1 - low1)
  return -5.0f + (value + 32767.0f) * 10.0f / (32767.0f * 2.0f);
}
}  // namespace

JoystickHandler::JoystickHandler() : joy_context_(libenjoy_init()) {
  Scan();
  for (auto const& [id, joystick_sample] : joystick_samples_) {
    Open(id);
  }
}

JoystickHandler::~JoystickHandler() {
  for (auto [id, joystick_raw] : joysticks_) {
    Close(id);
  }
  libenjoy_close(joy_context_);
}

std::vector<Input::Info> JoystickHandler::ListInputs() const {
  std::vector<Input::Info> joystick_infos;
  for (auto const& [id, joystick_sample] : joystick_samples_) {
    joystick_infos.push_back(joystick_sample.info);
  }
  return joystick_infos;
}

Input::Samples JoystickHandler::Poll() {
  libenjoy_event event;
  while (libenjoy_poll(joy_context_, &event)) {
    switch (event.type) {
      case LIBENJOY_EV_AXIS:
        HandleAxis(event.joy_id, event.part_id, event.data);
        break;
      case LIBENJOY_EV_BUTTON:
        HandleButton(event.joy_id, event.part_id, event.data == 1);
        break;
      case LIBENJOY_EV_CONNECTED:
        HandleConnection(event.joy_id, event.data == 1);
        break;
    }
  }

  Input::Samples samples;
  for (const auto& [id, sample] : joystick_samples_) {
    samples.push_back(sample);
  }
  return samples;
}

void JoystickHandler::SetConfig(int id, Input::Config config) {
  if (!config.enabled) {
    Close(static_cast<unsigned>(id));
  }
  // TODO(Menno 01.05.2024) Add a way to re-open the joystick once it's re-enabled.
}

void JoystickHandler::HandleAxis(unsigned id, unsigned axis, int value) {
  auto sample_it = joystick_samples_.find(id);
  if (sample_it == joystick_samples_.end() || (axis > sample_it->second.axes.size())) {
    return;
  }
  sample_it->second.axes[axis].value = RemapAxisRange(static_cast<float>(value));
}

void JoystickHandler::HandleButton(unsigned id, unsigned button, bool pressed) {
  auto sample_it = joystick_samples_.find(id);
  if (sample_it == joystick_samples_.end() || (button > sample_it->second.buttons.size())) {
    return;
  }
  sample_it->second.buttons[button].pressed = pressed;
}

void JoystickHandler::HandleConnection(unsigned, bool) {
  // TODO (Menno 01.05.2024) Handle dynamic connection/disconnection?
}

void JoystickHandler::Scan() {
  libenjoy_enumerate(joy_context_);
  auto list = libenjoy_get_info_list(joy_context_);

  for (int i = 0; i < list->count; i++) {
    auto entry = list->list[i];
    joystick_samples_[entry->id] = {
        .info = {.id = static_cast<int>(entry->id), .name = entry->name, .type = Input::Type::kJoystick}};
  }

  libenjoy_free_info_list(list);
}

void JoystickHandler::Open(unsigned id) {
  if (joysticks_.contains(id)) {
    LOG(WARNING) << "Joystick already open, refusing to open ID=" << id;
    return;
  }

  auto joystick_raw = libenjoy_open_joystick(joy_context_, id);
  if (!joystick_raw) {
    LOG(WARNING) << "Joystick open failed ID=" << id;
    return;
  }
  joysticks_.emplace(id, joystick_raw);

  if (auto sample_it = joystick_samples_.find(id); sample_it != joystick_samples_.end()) {
    int axes_count = libenjoy_get_axes_num(joystick_raw);
    sample_it->second.axes.resize(axes_count);
    int button_count = libenjoy_get_buttons_num(joystick_raw);
    sample_it->second.buttons.resize(button_count);
  }
}

void JoystickHandler::Close(unsigned int id) {
  auto joystick_raw_it = joysticks_.find(id);
  if (joystick_raw_it == joysticks_.end()) {
    LOG(WARNING) << "Joystick unknown, refusing to close ID=" << id;
    return;
  }

  libenjoy_close_joystick(joystick_raw_it->second);
}

#include "joystick_handler.h"

#include <absl/log/log.h>

#include <iostream>
#include <numeric>

#include "game/input/input.h"
#include "libenjoy.h"

namespace {
float RemapAxisRange(float value) {
  // low2 + (pressed - low1) * (high2 - low2) / (high1 - low1)
  return -5.0f + (value + 32767.0f) * 10.0f / (32767.0f * 2.0f);
}
}  // namespace

JoystickHandler::JoystickHandler() : joy_context_(libenjoy_init()) {
  ScanAll();
  for (auto const& [id, input] : joystick_inputs_) {
    Open(id);
  }
}

JoystickHandler::~JoystickHandler() {
  for (auto [id, joystick_raw] : joysticks_) {
    Close(id);
  }
  libenjoy_close(joy_context_);
}

InputList JoystickHandler::Poll() {
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

  InputList inputs;
  for (const auto& [id, input] : joystick_inputs_) {
    inputs.push_back(input);
  }
  return inputs;
}

void JoystickHandler::SetConfig(int id, Input::Config config) {
  if (!config.enabled) {
    Close(static_cast<unsigned>(id));
  }
  // TODO(Menno 01.05.2024) Add a way to re-open the joystick once it's re-enabled.
}

void JoystickHandler::HandleAxis(unsigned id, unsigned axis, int value) {
  auto input_it = joystick_inputs_.find(id);
  if (input_it == joystick_inputs_.end() || (axis > input_it->second->axes.size())) {
    return;
  }
  input_it->second->axes[axis].value = RemapAxisRange(static_cast<float>(value));
  // TODO(Menno 02.05.2024) Detect activity here
  //  if (de) {
  //    input_it->second->last_activity = std::chrono::steady_clock::now();
  //  }
}

void JoystickHandler::HandleButton(unsigned id, unsigned button, bool pressed) {
  auto input_it = joystick_inputs_.find(id);
  if (input_it == joystick_inputs_.end() || (button > input_it->second->buttons.size())) {
    return;
  }
  auto& button_ref = input_it->second->buttons[button];
  button_ref.down = pressed;
  button_ref.changed = true;
}

void JoystickHandler::HandleConnection(unsigned, bool) {
  // TODO (Menno 01.05.2024) Handle dynamic connection/disconnection?
}

void JoystickHandler::ScanAll() {
  libenjoy_enumerate(joy_context_);
  auto list = libenjoy_get_info_list(joy_context_);

  for (int i = 0; i < list->count; i++) {
    auto entry = list->list[i];
    if (!joystick_inputs_.contains(entry->id)) {
      auto input = std::make_shared<Input>();
      input->id = static_cast<int>(entry->id);
      input->name = entry->name;
      input->type = Input::Type::kJoystick;
      joystick_inputs_.emplace(input->id, input);
    }
  }

  libenjoy_free_info_list(list);
}

void JoystickHandler::Scan(unsigned id) {
  auto input_it = joystick_inputs_.find(id);
  if (input_it == joystick_inputs_.end()) {
    return;
  }
  auto joystick_raw_it = joysticks_.find(id);
  if (joystick_raw_it == joysticks_.end()) {
    return;
  }

  auto joystick_raw = joystick_raw_it->second;
  auto& input = input_it->second;

  int axes_count = libenjoy_get_axes_num(joystick_raw);
  input->axes.resize(axes_count);
  int count = 0;
  std::for_each(std::begin(input->axes), std::end(input->axes),
                [&count](auto& axis) { axis.name = std::format("Axis{}", count); });

  int button_count = libenjoy_get_buttons_num(joystick_raw_it->second);
  input->buttons.resize(button_count);
  count = 0;
  std::for_each(std::begin(input->buttons), std::end(input->buttons),
                [&count](auto& button) { button.name = std::format("Button{}", count); });
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

  Scan(id);
}

void JoystickHandler::Close(unsigned int id) {
  auto joystick_raw_it = joysticks_.find(id);
  if (joystick_raw_it == joysticks_.end()) {
    LOG(WARNING) << "Joystick unknown, refusing to close ID=" << id;
    return;
  }

  libenjoy_close_joystick(joystick_raw_it->second);
}

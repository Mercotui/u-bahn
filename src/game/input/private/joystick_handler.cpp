#include "game/input/private/joystick_handler.h"

#include <absl/log/log.h>

#include <chrono>
#include <iostream>
#include <memory>

#include "game/input/input.h"
#include "libenjoy/libenjoy.h"

namespace {
using std::chrono::steady_clock;
constexpr auto kScanInterval = std::chrono::seconds(2);

float RemapAxisRange(float value) {
  // low2 + (pressed - low1) * (high2 - low2) / (high1 - low1)
  return -1.0f + (value + 32767.0f) * 2.0f / (32767.0f * 2.0f);
}
}  // namespace

JoystickHandler::JoystickHandler() : joy_context_(libenjoy_init()) {
  ScanAll();
  for (auto const& [id, input] : joystick_inputs_) {
    Open(id);
  }
}

JoystickHandler::~JoystickHandler() {
  while (!joysticks_.empty()) {
    Close(std::begin(joysticks_)->first);
  }

  libenjoy_close(joy_context_);
}

InputList JoystickHandler::Poll() {
  if ((scan_timestamp_ + kScanInterval) < steady_clock::now()) {
    CheckReconnect();
  }

  // clear all activity flags
  for (auto& input : joystick_inputs_) {
    input.second->active = false;
    for (auto& axis : input.second->axes) {
      axis.active = false;
    }
    for (auto& button : input.second->buttons) {
      button.changed = false;
    }
  }

  // Creating the timepoint here is not great, as it has no correlation with the actual sample times, but I think
  // windows does not provide event timestamps, linux does, but we want to cater to the lowest common denominator.
  // We create one timepoint per Poll call to avoid getting a time diff between samples.
  const auto sample_timepoint = std::chrono::steady_clock::now();

  libenjoy_event event;
  while (libenjoy_poll(joy_context_, &event)) {
    switch (event.type) {
      case LIBENJOY_EV_AXIS:
        HandleAxis(event.joy_id, event.part_id, event.data, sample_timepoint);
        break;
      case LIBENJOY_EV_BUTTON:
        HandleButton(event.joy_id, event.part_id, event.data == 1);
        break;
      case LIBENJOY_EV_CONNECTED:
        HandleConnection(event.joy_id, event.data == 1);
        break;
      default:
        // Unknown event type
        ABSL_ASSERT(false);
    }
  }

  InputList inputs;
  for (const auto& [id, input] : joystick_inputs_) {
    if (input->config.enabled) {
      inputs.push_back(input);
    }
  }
  return inputs;
}

void JoystickHandler::SetConfig(const int id, const Input::Config config) {
  const auto input_it = joystick_inputs_.find(id);
  if (input_it == joystick_inputs_.end()) {
    return;
  }
  const bool previously_enabled = input_it->second->config.enabled;
  input_it->second->config = config;

  if (previously_enabled && !config.enabled) {
    Close(static_cast<unsigned>(id));
  } else if (!previously_enabled && config.enabled) {
    Open(static_cast<unsigned>(id));
  }
}

void JoystickHandler::HandleAxis(const unsigned id, const unsigned axis, const int value,
                                 const InputAxisHelpers::SampleTimePoint time_point) {
  const auto input_it = joystick_inputs_.find(id);
  if (input_it == joystick_inputs_.end() || (axis > input_it->second->axes.size())) {
    return;
  }

  auto& axis_ref = input_it->second->axes[axis];
  axis_ref.value = RemapAxisRange(static_cast<float>(value));

  const auto activity_it = axis_activity_.find(JoystickAxisKey{.id = id, .axis = axis});
  if (activity_it == axis_activity_.end()) {
    axis_ref.active = false;
    return;
  }
  axis_ref.active = activity_it->second.Detect({.time_point = time_point, .value = axis_ref.value});
  // if this axis is active then the joystick is active
  input_it->second->active |= axis_ref.active;
}

void JoystickHandler::HandleButton(const unsigned id, const unsigned button, const bool pressed) {
  const auto input_it = joystick_inputs_.find(id);
  if (input_it == joystick_inputs_.end() || (button > input_it->second->buttons.size())) {
    return;
  }
  auto& button_ref = input_it->second->buttons[button];
  button_ref.down = pressed;
  button_ref.changed = true;

  // if the button is changed then this joystick is active
  input_it->second->active = true;
}

void JoystickHandler::HandleConnection(const unsigned id, const bool connected) {
  if (!connected) {
    if (joysticks_.contains(id)) {
      // Note that we keep track of the joysticks that were open when they get disconnected,
      // so that we can reopen them in case they get re-connected.
      reconnect_joysticks_.insert(id);
      Close(id);
    }

    if (const auto input_it = joystick_inputs_.find(id); input_it != joystick_inputs_.end()) {
      const auto axes_count = input_it->second->axes.size();
      for (auto axis_index = 0; axis_index < axes_count; axis_index++) {
        // remove the axis activity detectors registered for this joystick
        axis_activity_.erase(JoystickAxisKey(id, axis_index));
      }
      joystick_inputs_.erase(input_it);
    }
  }
}

void JoystickHandler::CheckReconnect() {
  ScanAll();
  // Open any detected joysticks that should be reconnected
  std::erase_if(reconnect_joysticks_, [&](const auto& id) {
    if (joystick_inputs_.contains(id)) {
      Open(id);
      return true;
    }
    return false;
  });
}

void JoystickHandler::ScanAll() {
  scan_timestamp_ = steady_clock::now();
  libenjoy_enumerate(joy_context_);
  const auto list = libenjoy_get_info_list(joy_context_);

  for (int i = 0; i < list->count; i++) {
    if (const auto entry = list->list[i]; !joystick_inputs_.contains(entry->id)) {
      auto input = std::make_shared<Input>();
      input->id = static_cast<int>(entry->id);
      input->name = entry->name;
      input->type = Input::Type::kJoystick;
      joystick_inputs_.emplace(input->id, input);
    }
  }

  libenjoy_free_info_list(list);
}

void JoystickHandler::ScanFeatures(unsigned id) {
  const auto input_it = joystick_inputs_.find(id);
  if (input_it == joystick_inputs_.end()) {
    return;
  }
  auto joystick_raw_it = joysticks_.find(id);
  if (joystick_raw_it == joysticks_.end()) {
    return;
  }

  const auto joystick_raw = joystick_raw_it->second;
  const auto& input = input_it->second;

  const int axes_count = libenjoy_get_axes_num(joystick_raw);
  input->axes.resize(axes_count);
  int count = 0;
  std::ranges::for_each(input->axes, [&](auto& axis) {
    axis.name = std::format("Axis {}", count + 1);
    const auto logging_prefix = std::format("joy{}axis{}", id, count + 1);
    axis_activity_.emplace(JoystickAxisKey(id, count), logging_prefix);
    count++;
  });

  const int button_count = libenjoy_get_buttons_num(joystick_raw_it->second);
  input->buttons.resize(button_count);
  count = 1;
  std::ranges::for_each(input->buttons, [&count](auto& button) { button.name = std::format("Button {}", count++); });
}

void JoystickHandler::Open(const unsigned id) {
  if (joysticks_.contains(id)) {
    LOG(WARNING) << "Joystick already open, refusing to open ID=" << id;
    return;
  }

  const auto joystick_raw = libenjoy_open_joystick(joy_context_, id);
  if (!joystick_raw) {
    LOG(WARNING) << "Joystick open failed ID=" << id;
    return;
  }
  joysticks_.emplace(id, joystick_raw);

  ScanFeatures(id);
}
void JoystickHandler::Close(const unsigned int id) {
  const auto joystick_raw_it = joysticks_.find(id);
  if (joystick_raw_it == joysticks_.end()) {
    LOG(WARNING) << "Joystick unknown, refusing to close ID=" << id;
    return;
  }

  libenjoy_close_joystick(joystick_raw_it->second);
  joysticks_.erase(joystick_raw_it);
}

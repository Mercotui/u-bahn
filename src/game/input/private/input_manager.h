#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "game/input/input.h"
#include "game/input/input_manager_interface.h"
#include "platform/platform.h"

class InputManager : public InputManagerInterface {
 public:
  using DeviceHandlers = std::unordered_map<Input::Type, std::unique_ptr<InputManagerInterface>>;

  explicit InputManager(DeviceHandlers device_handlers) : device_handlers_(std::move(device_handlers)){};

  InputList Poll() override;

  void SetConfig(int id, Input::Config) override;

 private:
  DeviceHandlers device_handlers_;
  int current_input_{};
};

#pragma once

#include <vector>

#include "game/input/input.h"
#include "game/input/input_manager_interface.h"
#include "libenjoy.h"

class Joystick {
 public:
  struct JoyAxis {
    float value;
  };

  Joystick() = delete;
  ~Joystick() = default;

 private:
  explicit Joystick(int id);
};

class JoystickHandler final : public InputManagerInterface {
 public:
  JoystickHandler();
  ~JoystickHandler() override;

  [[nodiscard]] std::vector<Input::Info> ListInputs() const override;

  void Poll() override;

  void SetConfig(int id, Input::Config) override;

 private:
  libenjoy_context* joy_context_;
};
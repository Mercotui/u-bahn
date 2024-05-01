#pragma once

#include <map>
#include <vector>

#include "game/input/input.h"
#include "game/input/input_manager_interface.h"
#include "libenjoy.h"

class JoystickHandler final : public InputManagerInterface {
 public:
  JoystickHandler();
  ~JoystickHandler() override;

  [[nodiscard]] std::vector<Input::Info> ListInputs() const override;

  Input::Samples Poll() override;

  void SetConfig(int id, Input::Config) override;

 private:
  void HandleAxis(unsigned id, unsigned axis, int value);
  void HandleButton(unsigned id, unsigned button, bool pressed);
  void HandleConnection(unsigned id, bool connected);
  void Scan();
  void Open(unsigned id);
  void Close(unsigned id);

  libenjoy_context* joy_context_;
  std::map<unsigned, Input::Sample> joystick_samples_;
  std::map<unsigned, libenjoy_joystick*> joysticks_;
};
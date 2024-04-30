#pragma once

#include "game/input/input_manager_interface.h"

namespace RaylibInputHandlers {
class Keyboard final : public InputManagerInterface {
 public:
  [[nodiscard]] std::vector<Input::Info> ListInputs() const override;
  void Poll() override;

  void SetConfig(int id, Input::Config config) override;
};

class Mouse final : public InputManagerInterface {
 public:
  [[nodiscard]] std::vector<Input::Info> ListInputs() const override;
  void Poll() override;

  void SetConfig(int id, Input::Config config) override;
};

class Touch final : public InputManagerInterface {
 public:
  [[nodiscard]] std::vector<Input::Info> ListInputs() const override;
  void Poll() override;

  void SetConfig(int id, Input::Config config) override;
};

class Gamepad final : public InputManagerInterface {
 public:
  [[nodiscard]] std::vector<Input::Info> ListInputs() const override;
  void Poll() override;

  void SetConfig(int id, Input::Config config) override;
};
}  // namespace RaylibInputHandlers
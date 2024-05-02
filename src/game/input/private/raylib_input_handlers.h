#pragma once

#include <memory>

#include "game/input/input_manager_interface.h"

namespace RaylibInputHandlers {
class Keyboard final : public InputManagerInterface {
 public:
  Keyboard();

  InputList Poll() override;

  void SetConfig(int, Input::Config) override {};

 private:
  std::shared_ptr<Input> input_;
};

class Mouse final : public InputManagerInterface {
 public:
  Mouse();

  InputList Poll() override;

  void SetConfig(int, Input::Config) override {};

 private:
  std::shared_ptr<Input> input_;
};

class Touch final : public InputManagerInterface {
 public:
  Touch();

  InputList Poll() override;

  void SetConfig(int, Input::Config) override {};

 private:
  std::shared_ptr<Input> input_;
};

class Gamepad final : public InputManagerInterface {
 public:
  Gamepad();

  InputList Poll() override;

  void SetConfig(int id, Input::Config config) override;

 private:
  std::shared_ptr<Input> input_;
};
}  // namespace RaylibInputHandlers

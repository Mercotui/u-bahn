#pragma once

#include <raylib.h>

#include <memory>
#include <string>

//! Forward Declared, defined in game/input/input_manager_interface.h
class InputManagerInterface;
class ControlSchemeMapper;

class Game {
 public:
  Game();
  ~Game();

  bool Loop();

 private:
  bool running_{true};
  float train_velocity_{0.0f};
  float train_location_{0.0f};

  Model model_{};
  Camera3D camera_{};
  std::string active_input_{};
  std::unique_ptr<InputManagerInterface> input_;
  std::unique_ptr<ControlSchemeMapper> controls_mapper_;
};

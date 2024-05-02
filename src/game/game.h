#pragma once

#include <raylib.h>

#include <memory>

#include "input/input_manager_interface.h"

class Game {
 public:
  Game();
  ~Game();

  bool Loop();

 private:
  bool running_{true};

  Model model_{};
  Camera3D camera_{};
  std::unique_ptr<InputManagerInterface> input_;
};

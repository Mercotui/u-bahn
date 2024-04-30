#pragma once
#include <raylib.h>

class Game {
 public:
  Game();
  ~Game();

  bool Loop();

 private:
  bool running_{true};

  Model model_{};
  Camera3D camera_{};
};

#pragma once

#include <raylib.h>

#include <array>
#include <memory>

#include "game/control/control.h"
#include "game/world/rails.h"

class Train {
 public:
  explicit Train(const Rails& rails, Rails::Location location);
  ~Train();

  void Control(const Control::TrainControls& controls);
  [[nodiscard]] float Velocity() const { return velocity_; }

  void Draw();

 private:
  const Rails& rails_;
  Rails::Location location_;
  float velocity_{0.0f};

  Model model_{};
};

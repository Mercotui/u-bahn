#pragma once

#include <raylib.h>

#include <array>
#include <memory>
#include <utility>

#include "game/control/control.h"
#include "game/world/rails.h"
#include "game/world/units.h"

class Train {
 public:
  explicit Train(const Rails& rails, Rails::Location location);
  ~Train();

  void Control(const Control::TrainControls& controls, Units::TimeDelta time);
  [[nodiscard]] Units::Speed Speed() const { return speed_; }

  void Draw() const;

 private:
  const Rails& rails_;
  std::pair<Rails::Location, Rails::Location> location_;
  double direction_{1.0};
  Units::Speed speed_{};

  Model model_{};
};

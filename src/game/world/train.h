#pragma once

#include <raylib.h>

#include <array>
#include <memory>
#include <utility>
#include <vector>

#include "game/control/control.h"
#include "game/world/rails.h"
#include "game/world/units.h"

class Train {
 public:
  explicit Train(const Rails& rails, Rails::Location location, int car_count);
  ~Train();

  void Control(const Control::TrainControls& controls, Units::TimeDelta time);
  [[nodiscard]] Units::Speed Speed() const { return speed_; }

  void Draw() const;
  void DrawDebug() const;

 private:
  const Rails& rails_;
  using Car = std::pair<Rails::Location, Rails::Location>;
  std::vector<Car> cars_;
  double direction_{1.0};
  Units::Speed speed_{};

  Model model_{};
};

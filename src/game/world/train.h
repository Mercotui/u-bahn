#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "game/control/control.h"
#include "game/world/rails.h"
#include "game/world/units.h"
#include "third_party/raylib/raylib.h"

class Train {
 public:
  explicit Train(const Rails& rails, const Rails::Location& location, int car_count);
  ~Train();

  void Control(const Control::TrainControls& controls, Units::TimeDelta time);
  [[nodiscard]] Units::Speed Speed() const { return speed_; }

  World::WorldSpaceCoordinates GetCenterPoint() const;
  void Draw() const;
  void DrawDebug() const;

 private:
  const Rails& rails_;
  using Car = std::pair<Rails::Location, Rails::Location>;
  std::vector<Car> cars_;
  double direction_{1.0};
  Units::Speed speed_{};

  Raylib::Model model_{};
};

#pragma once

#include <array>
#include <memory>
#include <utility>

#include "game/control/control.h"
#include "game/world/rails.h"
#include "game/world/units.h"

//! Forward Declared, defined in raylib.h
class Model;

class Train {
 public:
  explicit Train(const Rails& rails, Rails::Location location);
  ~Train();

  void Control(const Control::TrainControls& controls, Units::TimeDelta time);
  [[nodiscard]] Units::Speed Speed() const { return speed_; }
  World::WorldSpaceCoordinates WorldSpaceLocation() const;

  void Draw() const;

 private:
  const Rails& rails_;
  std::pair<Rails::Location, Rails::Location> location_;
  double direction_{1.0};
  Units::Speed speed_{};

  std::unique_ptr<Model> model_{};
};

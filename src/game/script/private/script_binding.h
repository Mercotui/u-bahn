#pragma once

#include <memory>
#include <vector>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>  // cpplint falsely flags this as a C header

#include "game/world/train.h"

class ScriptBinding {
 public:
  struct SpeedRange {
    Units::Speed minimum;
    Units::Speed maximum;
  };

  struct Trigger {
    World::WorldSpaceCoordinates center;
    Units::Distance radius;
    SpeedRange speed_range;
    sol::function on_trigger;
    bool triggered{false};
  };

  static std::shared_ptr<ScriptBinding> Instance();

  void CheckTriggers(const Train& train);

  void AddTrigger(double point_x, double point_y, double radius, double speed_min, double speed_max,
                  sol::function callback);

  void DrawDebug();

 private:
  std::vector<Trigger> triggers_;
};

#pragma once

#include <string>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>  // cpplint falsely flags this as a C header

#include "game/world/train.h"
#include "game/world/units.h"
#include "game/world/world.h"

class ScriptInterpreter {
 public:
  explicit ScriptInterpreter(const std::string& raw_state);

  void CheckTriggers(const Train&);
  void DrawDebug();

 public:
  sol::state lua;
};

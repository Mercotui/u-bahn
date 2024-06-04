#pragma once

#include <mp-units/systems/si/si.h>

#include "game/world/units.h"

namespace World {
inline constexpr struct origin : mp_units::absolute_point_origin<origin, mp_units::isq::distance> {
} origin;

using Coordinate = mp_units::quantity_point<Units::Distance::unit, origin>;

struct WorldSpaceCoordinates {
  Coordinate x{};
  Coordinate y{};
  Coordinate z{};

  [[nodiscard]] bool operator==(const WorldSpaceCoordinates& other) const {
    return x == other.x && y == other.y && z == other.z;
  }
};
}  // namespace World

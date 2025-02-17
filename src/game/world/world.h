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

  /**
   * Calculate the average between this and other
   * @param other the other
   * @return the average between this and other
   */
  [[nodiscard]] WorldSpaceCoordinates average(const WorldSpaceCoordinates& other) const {
    WorldSpaceCoordinates result;
    result.x = x + (other.x - x) * 0.5;
    result.y = y + (other.y - y) * 0.5;
    result.z = z + (other.z - z) * 0.5;
    return result;
  }
};
}  // namespace World

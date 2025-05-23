// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <mp-units/systems/isq/isq.h>

#include <memory>

#include "game/world/units.h"

//! Forward Declared, defined in game/world/rails.h
class Rails;

namespace World {
inline constexpr struct origin : mp_units::absolute_point_origin<origin, mp_units::isq::distance> {
} origin;

using Coordinate = mp_units::quantity_point<Units::Distance::unit, origin>;

struct WorldSpaceCoordinates {
  Coordinate x{};
  Coordinate y{};
  Coordinate z{};

  [[nodiscard]] auto operator<=>(const WorldSpaceCoordinates& other) const = default;

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

/**
 * Load the world from a stream containing binary protobuf data
 * @param data The binary protobuf Section message data
 * @return the parsed rails structure if the message was valid, otherwise nullptr
 */
std::unique_ptr<Rails> Load(std::istream* data);
}  // namespace World

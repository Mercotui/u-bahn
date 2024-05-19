#pragma once

class World {
 public:
  struct WorldSpaceCoordinates {
    float x{};
    float y{};
    float z{};

    [[nodiscard]] bool operator==(const WorldSpaceCoordinates& other) const {
      return x == other.x && y == other.y && z == other.z;
    }
  };
};

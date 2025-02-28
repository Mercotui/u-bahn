// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/world/world.h"

#include <gtest/gtest.h>
#include <mp-units/systems/si/si.h>

#include <fstream>
#include <sstream>

#include "game/world/rails.h"

namespace {
constexpr auto kTestTrackPath = "world_unitest_test_track.bin";
using mp_units::si::metre;
using World::origin;
using World::WorldSpaceCoordinates;
}  // namespace

TEST(WorldTest, LoadFail) {
  // Check that an empty string is safely handled as invalid
  std::stringstream empty_data("");
  EXPECT_FALSE(World::Load(&empty_data));

  // Check that an empty string is safely handled as invalid
  std::stringstream garbage_data("This is a test message! Garbage! 🗑️");
  EXPECT_FALSE(World::Load(&garbage_data));
}

TEST(WorldTest, Load) {
  // Load a test track from a file
  std::ifstream data(kTestTrackPath, std::ios::in | std::ios::binary);

  Rails::Location location_1 = {.segment = {.id = 1}};
  Rails::Location location_2 = {.segment = {.id = 2}};
  Rails::Location location_3 = {.segment = {.id = 3}};
  Rails::Location location_4 = {.segment = {.id = 4}};
  Rails::Location location_5 = {.segment = {.id = 5}};

  WorldSpaceCoordinates point_1 = {.x = origin + -20.0 * metre, .y = origin + -20.0 * metre};
  WorldSpaceCoordinates point_2 = {.x = origin + -20.0 * metre, .y = origin + 20.0 * metre};
  WorldSpaceCoordinates point_3 = {.x = origin + 0.0 * metre, .y = origin + 0.0 * metre};
  WorldSpaceCoordinates point_4 = {.x = origin + 20.0 * metre, .y = origin + 0.0 * metre};
  WorldSpaceCoordinates point_5 = {.x = origin + 20.0 * metre, .y = origin + 0.0 * metre};

  const auto rails = World::Load(&data);
  EXPECT_TRUE(rails);
  EXPECT_EQ(rails->WorldSpace(location_1), point_1);
  EXPECT_EQ(rails->WorldSpace(location_2), point_2);
  EXPECT_EQ(rails->WorldSpace(location_3), point_3);
  EXPECT_EQ(rails->WorldSpace(location_4), point_4);
  EXPECT_EQ(rails->WorldSpace(location_5), point_5);
}

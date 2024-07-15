#include "game/world/rails.h"

#include <gtest/gtest.h>
#include <mp-units/math.h>

#include <source_location>

namespace {
using mp_units::si::metre;
using testing::ScopedTrace;
using WorldSpaceCoordinates = World::WorldSpaceCoordinates;
using World::origin;

constexpr auto kDistanceEpsilon = 0.00001 * metre;

//! It seems mp_units does not have a "near equals", or at least it is not yet documented.
void ExpectNear(Units::Distance lhs, Units::Distance rhs, Units::Distance epsilon = kDistanceEpsilon,
                std::source_location callee = std::source_location::current()) {
  ScopedTrace trace(callee.file_name(), callee.line(), "Failure occurred here");
  EXPECT_LE(mp_units::abs(lhs - rhs), epsilon);
}

//! It seems mp_units does not have a "near equals", or at least it is not yet documented.
void ExpectNear(WorldSpaceCoordinates lhs, WorldSpaceCoordinates rhs, Units::Distance epsilon = kDistanceEpsilon,
                std::source_location callee = std::source_location::current()) {
  ScopedTrace trace(callee.file_name(), callee.line(), "Failure occurred here");
  EXPECT_LE(mp_units::abs(lhs.x - rhs.x), epsilon);
  EXPECT_LE(mp_units::abs(lhs.y - rhs.y), epsilon);
  EXPECT_LE(mp_units::abs(lhs.z - rhs.z), epsilon);
}
}  // namespace

TEST(RailsTest, TraverseSingle) {
  Rails rails;
  // construct a straight rail segment between (0, 0) and (10, 0)
  Rails::SegmentId id = {.id = 1};
  WorldSpaceCoordinates start_point = {.x = origin - 10.0 * metre};
  WorldSpaceCoordinates end_point = {.x = origin + 10.0 * metre};
  rails.AddSegment(id, {start_point, end_point});

  struct Data {
    Units::Distance start{};
    Units::Distance distance{};
    Units::Distance result{};
  };

  constexpr std::array test_data = {
      Data{.start = -10.0 * metre, .distance = -1.0 * metre, .result = 0.0 * metre},
      Data{.start = 0.0 * metre, .distance = -2.0 * metre, .result = 0.0 * metre},
      Data{.start = 0.0 * metre, .distance = 2.0 * metre, .result = 2.0 * metre},
      Data{.start = 0.0 * metre, .distance = 20.0 * metre, .result = 20.0 * metre},
      Data{.start = 19.0 * metre, .distance = 1.0 * metre, .result = 20.0 * metre},
      Data{.start = 20.0 * metre, .distance = 1.0 * metre, .result = 20.0 * metre},
      Data{.start = 20.0 * metre, .distance = 1.0 * metre, .result = 20.0 * metre},
      Data{.start = 20.0 * metre, .distance = -20.0 * metre, .result = 0.0 * metre},
  };

  for (const auto& data : test_data) {
    // create a location at the start of this rail segment
    Rails::Location start_location{.segment = {.id = 1}, .intra_segment_location = data.start};

    const auto result_location = rails.Traverse(start_location, data.distance);
    EXPECT_EQ(result_location.segment, id);
    ExpectNear(result_location.intra_segment_location, data.result, kDistanceEpsilon);
  }
}

TEST(RailsTest, TraverseMultiple) {
  Rails rails;
  // construct a curved segment through (0, 0), (0, 10), (10, 10) and ending at (10, 0)
  // and then a straight rail segment back between (0, 0) and (10, 0)
  WorldSpaceCoordinates point_1 = {.x = origin + 0.0 * metre, .y = origin + 0.0 * metre};
  WorldSpaceCoordinates point_2 = {.x = origin + 0.0 * metre, .y = origin + 10.0 * metre};
  WorldSpaceCoordinates point_3 = {.x = origin + 10.0 * metre, .y = origin + 10.0 * metre};
  WorldSpaceCoordinates point_4 = {.x = origin + 10.0 * metre, .y = origin + 0.0 * metre};
  constexpr Rails::SegmentId id_1 = {.id = 1};
  constexpr Rails::SegmentId id_2 = {.id = 2};
  rails.AddSegment(id_1, {point_1, point_2, point_3, point_4}, id_2, id_2);
  rails.AddSegment(id_2, {point_4, point_1}, id_1, id_1);

  // create a location at the middle of segment 1
  Rails::Location original_location{.segment = {.id = 1}, .intra_segment_location = 5.0 * metre};

  struct Data {
    Units::Distance distance{};
    Units::Distance resulting_location{};
    Rails::SegmentId resulting_id;
  };

  constexpr std::array test_data = {
      Data{.distance = 10.0 * metre, .resulting_location = 15.0 * metre, .resulting_id = id_1},
      Data{.distance = 20.0 * metre, .resulting_location = 5.0 * metre, .resulting_id = id_2},
      Data{.distance = 28.0 * metre, .resulting_location = 3.0 * metre, .resulting_id = id_1},
  };

  for (const auto& data : test_data) {
    const auto location = rails.Traverse(original_location, data.distance);
    EXPECT_EQ(location.segment, data.resulting_id);
    // We are compounding a lot of errors here, so our expectations are quite lenient
    ExpectNear(location.intra_segment_location, data.resulting_location, 0.1 * metre);
  }
}

TEST(RailsTest, WorldSpace) {
  Rails rails;
  // construct a straight rail segment between (0, 0) and (10, 0)
  Rails::SegmentId id = {.id = 1};
  WorldSpaceCoordinates start_point = {};
  WorldSpaceCoordinates end_point = {.x = origin + 10.0 * metre};
  rails.AddSegment(id, {start_point, end_point});

  struct Data {
    Units::Distance intra_segment_location{};
    WorldSpaceCoordinates world_point;
  };

  constexpr std::array test_data_line = {
      Data{-2.0 * metre, {.x = origin + 0.0 * metre}},  Data{0.0 * metre, {.x = origin + 0.0 * metre}},
      Data{2.0 * metre, {.x = origin + 2.0 * metre}},   Data{5.0 * metre, {.x = origin + 5.0 * metre}},
      Data{10.0 * metre, {.x = origin + 10.0 * metre}}, Data{15.0 * metre, {.x = origin + 10.0 * metre}},
  };

  for (const auto& data : test_data_line) {
    Rails::Location location{.segment = {.id = 1}, .intra_segment_location = data.intra_segment_location};
    const auto point = rails.WorldSpace(location);
    ExpectNear(point, data.world_point);
  }

  // Approximate a unit-circle
  constexpr std::array test_data_circle = {
      Data{-2.0 * metre, {.x = origin + 0.0 * metre}},  Data{0.0 * metre, {.x = origin + 0.0 * metre}},
      Data{2.0 * metre, {.x = origin + 2.0 * metre}},   Data{5.0 * metre, {.x = origin + 5.0 * metre}},
      Data{10.0 * metre, {.x = origin + 10.0 * metre}}, Data{15.0 * metre, {.x = origin + 10.0 * metre}},
  };

  for (const auto& data : test_data_circle) {
    Rails::Location location{.segment = {.id = 1}, .intra_segment_location = data.intra_segment_location};
    const auto point = rails.WorldSpace(location);
    ExpectNear(point, data.world_point);
  }
}

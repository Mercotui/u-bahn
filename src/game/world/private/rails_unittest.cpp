// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/world/rails.h"

#include <gtest/gtest.h>
#include <mp-units/math.h>

#include <numbers>
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
  ScopedTrace trace(callee.file_name(), static_cast<int>(callee.line()), "Failure occurred here");
  EXPECT_LE(mp_units::abs(lhs - rhs), epsilon);
}

//! It seems mp_units does not have a "near equals", or at least it is not yet documented.
void ExpectNear(WorldSpaceCoordinates lhs, WorldSpaceCoordinates rhs, Units::Distance epsilon = kDistanceEpsilon,
                std::source_location callee = std::source_location::current()) {
  ScopedTrace trace(callee.file_name(), static_cast<int>(callee.line()), "Failure occurred here");
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
    std::source_location src_loc = std::source_location::current();
    Units::Distance start{};
    Units::Distance distance{};
    Units::Distance result_distance{};
    bool result_status{};
  };

  constexpr std::array test_data = {
      Data{.start = -10.0 * metre, .distance = -1.0 * metre, .result_distance = 0.0 * metre, .result_status = false},
      Data{.start = 0.0 * metre, .distance = -2.0 * metre, .result_distance = 0.0 * metre, .result_status = false},
      Data{.start = 0.0 * metre, .distance = 2.0 * metre, .result_distance = 2.0 * metre, .result_status = true},
      Data{.start = 0.0 * metre, .distance = 20.0 * metre, .result_distance = 20.0 * metre, .result_status = true},
      Data{.start = 10.0 * metre, .distance = 0.0 * metre, .result_distance = 10.0 * metre, .result_status = true},
      Data{.start = 19.0 * metre, .distance = 1.0 * metre, .result_distance = 20.0 * metre, .result_status = true},
      Data{.start = 20.0 * metre, .distance = 1.0 * metre, .result_distance = 20.0 * metre, .result_status = false},
      Data{.start = 30.0 * metre, .distance = 1.0 * metre, .result_distance = 20.0 * metre, .result_status = false},
      Data{.start = 20.0 * metre, .distance = -20.0 * metre, .result_distance = 0.0 * metre, .result_status = true},
  };

  for (const auto& data : test_data) {
    ScopedTrace trace(data.src_loc.file_name(), static_cast<int>(data.src_loc.line()), "Expectation declared here");

    // create a location at the start of this rail segment
    Rails::Location start_location{.segment = {.id = 1}, .intra_segment_location = data.start};

    const auto [result_location, result_status] = rails.Traverse(start_location, data.distance);
    EXPECT_EQ(result_location.segment, id);
    EXPECT_EQ(result_status, data.result_status);
    ExpectNear(result_location.intra_segment_location, data.result_distance, kDistanceEpsilon);
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
  rails.AddSegment(id_1, {point_1, point_2, point_3, point_4}, {{id_2}});
  rails.AddSegment(id_2, {point_4, point_1}, {{id_1}});

  // create a location at the middle of segment 1
  Rails::Location original_location{.segment = {.id = 1}, .intra_segment_location = 5.0 * metre};

  struct Data {
    std::source_location src_loc = std::source_location::current();
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
    ScopedTrace trace(data.src_loc.file_name(), static_cast<int>(data.src_loc.line()), "Expectation declared here");

    const auto [location, status] = rails.Traverse(original_location, data.distance);
    EXPECT_TRUE(status);
    EXPECT_EQ(location.segment, data.resulting_id);
    // We are compounding a lot of errors here, so our expectations are quite lenient
    ExpectNear(location.intra_segment_location, data.resulting_location, 0.1 * metre);
  }
}

TEST(RailsTest, TraverseMultipleBackwards) {
  Rails rails;
  // construct a curved segment through (0, 0), (0, 10), (10, 10) and ending at (10, 0)
  // and then a straight rail segment back between (0, 0) and (10, 0)
  WorldSpaceCoordinates point_1 = {.x = origin + 0.0 * metre, .y = origin + 0.0 * metre};
  WorldSpaceCoordinates point_2 = {.x = origin + 0.0 * metre, .y = origin + 10.0 * metre};
  WorldSpaceCoordinates point_3 = {.x = origin + 10.0 * metre, .y = origin + 10.0 * metre};
  WorldSpaceCoordinates point_4 = {.x = origin + 10.0 * metre, .y = origin + 0.0 * metre};
  constexpr Rails::SegmentId id_1 = {.id = 1};
  constexpr Rails::SegmentId id_2 = {.id = 2};
  rails.AddSegment(id_1, {point_1, point_2, point_3, point_4});
  rails.AddSegment(id_2, {point_1, point_4}, {{id_1, Rails::SegmentEndpoint::kBegin}}, {{id_1}});

  // create a location at the middle of segment 1
  Rails::Location original_location{.segment = {.id = 1},
                                    .intra_segment_location = 5.0 * metre,
                                    .intra_segment_direction = Rails::SegmentTraverseDirection::kBackward};

  struct Data {
    std::source_location src_loc = std::source_location::current();
    Units::Distance distance{};
    Units::Distance resulting_location{};
    Rails::SegmentId resulting_id;
  };

  constexpr std::array test_data = {
      Data{.distance = -10.0 * metre, .resulting_location = 15.0 * metre, .resulting_id = id_1},
      Data{.distance = -20.0 * metre, .resulting_location = 5.0 * metre, .resulting_id = id_2},
      Data{.distance = -28.0 * metre, .resulting_location = 3.0 * metre, .resulting_id = id_1},
  };

  for (const auto& data : test_data) {
    ScopedTrace trace(data.src_loc.file_name(), static_cast<int>(data.src_loc.line()), "Expectation declared here");

    const auto [location, status] = rails.Traverse(original_location, data.distance);
    EXPECT_TRUE(status);
    EXPECT_EQ(location.segment, data.resulting_id);
    // We are compounding a lot of errors here, so our expectations are quite lenient
    ExpectNear(location.intra_segment_location, data.resulting_location, 0.1 * metre);
  }
}

TEST(RailsTest, TraverseTurningLoop) {
  Rails rails;
  // A turning loop is any track that loops back onto itself after rotating 180 degrees, which causes the train to
  // travel the same track in the opposite direction. A line(A,B) and a triangle(B,C,D,B) at the end of that line
  // suffices for testing. Note the length of the triangle lines 1 and 3 are sqrt(2) ≈ 1.414
  WorldSpaceCoordinates point_A = {.x = origin + 0.0 * metre, .y = origin + 0.0 * metre};
  WorldSpaceCoordinates point_B = {.x = origin + 10.0 * metre, .y = origin + 0.0 * metre};
  WorldSpaceCoordinates point_C = {.x = origin + 11.0 * metre, .y = origin - 1.0 * metre};
  WorldSpaceCoordinates point_D = {.x = origin + 11.0 * metre, .y = origin + 1.0 * metre};
  constexpr Rails::SegmentId id_line = {.id = 1};
  constexpr Rails::SegmentId id_triangle_line_1 = {.id = 2};
  constexpr Rails::SegmentId id_triangle_line_2 = {.id = 3};
  constexpr Rails::SegmentId id_triangle_line_3 = {.id = 4};
  rails.AddSegment(id_line, {point_A, point_B});
  rails.AddSegment(id_triangle_line_1, {point_B, point_C}, {{id_line}});
  rails.AddSegment(id_triangle_line_2, {point_C, point_D}, {{id_triangle_line_1}});
  rails.AddSegment(id_triangle_line_3, {point_D, point_B}, {{id_triangle_line_2}}, {{id_line}});

  // create a location at the middle of segment 1
  Rails::Location original_location{.segment = {.id = 1}, .intra_segment_location = 5.0 * metre};

  struct Data {
    std::source_location src_loc = std::source_location::current();
    Units::Distance distance{};
    Units::Distance resulting_location{};
    Rails::SegmentId resulting_id;
  };

  using std::numbers::sqrt2_v;
  constexpr std::array test_data = {
      Data{.distance = 6.0 * metre, .resulting_location = 1.0 * metre, .resulting_id = id_triangle_line_1},
      Data{.distance = 7.414 * metre, .resulting_location = 1.0 * metre, .resulting_id = id_triangle_line_2},
      Data{.distance = 9.414 * metre, .resulting_location = 1.0 * metre, .resulting_id = id_triangle_line_3},
      // These expectations are traversing the original line backwards
      Data{.distance = 10.828 * metre, .resulting_location = 9.0 * metre, .resulting_id = id_line},
      Data{.distance = 14.828 * metre, .resulting_location = 5.0 * metre, .resulting_id = id_line},
      Data{.distance = 19.828 * metre, .resulting_location = 0.0 * metre, .resulting_id = id_line},
  };

  for (const auto& data : test_data) {
    ScopedTrace trace(data.src_loc.file_name(), static_cast<int>(data.src_loc.line()), "Expectation declared here");

    const auto [location, status] = rails.Traverse(original_location, data.distance);
    EXPECT_TRUE(status);
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
    std::source_location src_loc = std::source_location::current();
  };

  constexpr std::array test_data_line = {
      Data{-2.0 * metre, {.x = origin + 0.0 * metre}},  Data{0.0 * metre, {.x = origin + 0.0 * metre}},
      Data{2.0 * metre, {.x = origin + 2.0 * metre}},   Data{5.0 * metre, {.x = origin + 5.0 * metre}},
      Data{10.0 * metre, {.x = origin + 10.0 * metre}}, Data{15.0 * metre, {.x = origin + 10.0 * metre}},
  };

  for (const auto& data : test_data_line) {
    ScopedTrace trace(data.src_loc.file_name(), static_cast<int>(data.src_loc.line()), "Expectation declared here");

    Rails::Location location{.segment = {.id = 1}, .intra_segment_location = data.intra_segment_location};
    const auto point = rails.WorldSpace(location);
    ExpectNear(point, data.world_point);
  }
}

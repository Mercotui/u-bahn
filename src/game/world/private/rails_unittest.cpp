#include "game/world/rails.h"

#include <gtest/gtest.h>

namespace {
using WorldSpaceCoordinates = World::WorldSpaceCoordinates;
}

TEST(RailsTest, TraverseSingle) {
  Rails rails;
  // construct a straight rail segment between (0, 0) and (10, 0)
  Rails::SegmentId id = {.id = 1};
  World::WorldSpaceCoordinates start_point = {.x = -10.0f};
  World::WorldSpaceCoordinates end_point = {.x = 10.0f};
  rails.AddSegment(id, {start_point, end_point});

  struct Data {
    float start{};
    float distance{};
    float result{};
  };

  constexpr std::array test_data = {
      Data{.start = -10.0f, .distance = -1.0f, .result = 0.0f},
      Data{.start = 0.0f, .distance = -2.0f, .result = 0.0f},
      Data{.start = 0.0f, .distance = 2.0f, .result = 2.0f},
      Data{.start = 0.0f, .distance = 20.0f, .result = 20.0f},
      Data{.start = 19.0f, .distance = 1.0f, .result = 20.0f},
      Data{.start = 20.0f, .distance = 1.0f, .result = 20.0f},
      Data{.start = 20.0f, .distance = 1.0f, .result = 20.0f},
      Data{.start = 20.0f, .distance = -20.0f, .result = 0.0f},
  };

  for (const auto& data : test_data) {
    // create a location at the start of this rail segment
    Rails::Location start_location{.segment = {.id = 1}, .intra_segment_location = data.start};

    const auto result_location = rails.Traverse(start_location, data.distance);
    EXPECT_EQ(result_location.segment, id);
    EXPECT_EQ(result_location.intra_segment_location, data.result);
  }
}

TEST(RailsTest, TraverseMultiple) {
  Rails rails;
  // construct a curved segment through (0, 0), (0, 10), (10, 10) and ending at (10, 0)
  // and then a straight rail segment back between (0, 0) and (10, 0)
  World::WorldSpaceCoordinates point_1 = {.x = 0.0f, .y = 0.0f};
  World::WorldSpaceCoordinates point_2 = {.x = .0f, .y = 10.0f};
  World::WorldSpaceCoordinates point_3 = {.x = 10.0f, .y = 10.0f};
  World::WorldSpaceCoordinates point_4 = {.x = 10.0f, .y = 0.0f};
  constexpr Rails::SegmentId id_1 = {.id = 1};
  constexpr Rails::SegmentId id_2 = {.id = 2};
  rails.AddSegment(id_1, {point_1, point_2, point_3, point_4}, id_2, id_2);
  rails.AddSegment(id_2, {point_4, point_1}, id_1, id_1);

  // create a location at the middle of segment 1
  Rails::Location original_location{.segment = {.id = 1}, .intra_segment_location = 5.0f};

  struct Data {
    float distance{};
    float resulting_location{};
    Rails::SegmentId resulting_id;
  };

  constexpr std::array test_data = {
      Data{.distance = 10.0f, .resulting_location = 15.0f, .resulting_id = id_1},
      Data{.distance = 20.0f, .resulting_location = 5.0f, .resulting_id = id_2},
      Data{.distance = 28.0f, .resulting_location = 3.0f, .resulting_id = id_1},
  };

  for (const auto& data : test_data) {
    const auto location = rails.Traverse(original_location, data.distance);
    EXPECT_EQ(location.segment, data.resulting_id);
    EXPECT_NEAR(location.intra_segment_location, data.resulting_location, 0.01f);
  }
}

TEST(RailsTest, WorldSpace) {
  Rails rails;
  // construct a straight rail segment between (0, 0) and (10, 0)
  Rails::SegmentId id = {.id = 1};
  World::WorldSpaceCoordinates start_point = {};
  World::WorldSpaceCoordinates end_point = {.x = 10.0f};
  rails.AddSegment(id, {start_point, end_point});

  struct Data {
    float intra_segment_location{};
    WorldSpaceCoordinates world_point;
  };

  constexpr std::array test_data_line = {
      Data{-2.0f, {.x = 0.0f}}, Data{0.0f, {.x = 0.0f}},   Data{2.0f, {.x = 2.0f}},
      Data{5.0f, {.x = 5.0f}},  Data{10.0f, {.x = 10.0f}}, Data{15.0f, {.x = 10.0f}},
  };

  for (const auto& data : test_data_line) {
    Rails::Location location{.segment = {.id = 1}, .intra_segment_location = data.intra_segment_location};
    const auto point = rails.WorldSpace(location);
    EXPECT_EQ(point, data.world_point);
  }

  // Approximate a unit-circle
  constexpr std::array test_data_circle = {
      Data{-2.0f, {.x = 0.0f}}, Data{0.0f, {.x = 0.0f}},   Data{2.0f, {.x = 2.0f}},
      Data{5.0f, {.x = 5.0f}},  Data{10.0f, {.x = 10.0f}}, Data{15.0f, {.x = 10.0f}},
  };

  for (const auto& data : test_data_circle) {
    Rails::Location location{.segment = {.id = 1}, .intra_segment_location = data.intra_segment_location};
    const auto point = rails.WorldSpace(location);
    EXPECT_EQ(point, data.world_point);
  }
}

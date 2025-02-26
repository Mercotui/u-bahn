// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/world/private/rail_parser.h"
#include "game/world/rails.h"
#include <gtest/gtest.h>

TEST(RailParserTest, ParseNull) {
  const auto rails1 = RailParser::Parse(nlohmann::json::parse("{}"));
  EXPECT_TRUE(rails1);
  const auto rails2 = RailParser::Parse(nlohmann::json());
  EXPECT_TRUE(rails2);
}

TEST(RailParserTest, ParseTurningLoop) {
  const auto rails = RailParser::Parse(nlohmann::json::parse(
      R"({
        "points": []
      })"
      ));
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

// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/world/private/rail_parser.h"

#include <absl/log/log.h>

#include "game/world/rails.h"
#include "game/world/world.h"
#include "rail_segment.h"

namespace {
using mp_units::si::metre;

enum class UnitCircleQuadrant {
  kTopRight,
  kTopLeft,
  kBottomLeft,
  kBottomRight,
};

std::optional<World::WorldSpaceCoordinates> ParsePoint(const nlohmann::json& point) {
  if (!point.is_array()) {
    LOG(ERROR) << "Rail parsing failed, a value in 'points' is not array but instead:" << point;
    return std::nullopt;
  }
  if (point.size() != 3) {
    LOG(ERROR) << "Rail parsing failed, a value in 'points' should contain 3 coordinates but instead contains:"
               << point;
    return std::nullopt;
  }

  if (!(point[0].is_number() && point[1].is_number() && point[2].is_number())) {
    LOG(ERROR) << "Rail segment parsing failed, point contains coordinate that is not a number: " << point;
    return std::nullopt;
  }
  World::WorldSpaceCoordinates result;
  result.x = World::origin + point[0].get<double>() * metre;
  result.y = World::origin + point[1].get<double>() * metre;
  result.z = World::origin + point[2].get<double>() * metre;
  return result;
}

std::vector<World::WorldSpaceCoordinates> ParsePoints(const nlohmann::json& points) {
  if (points.is_null()) {
    LOG(ERROR) << "Rail parsing failed, could not find 'points' entry";
    return {};
  }
  if (!points.is_array()) {
    LOG(ERROR) << "Rail parsing failed, 'points' entry is not array but instead:" << points;
    return {};
  }

  std::vector<World::WorldSpaceCoordinates> result;
  for (const auto& point : points) {
    const auto converted_point = ParsePoint(point);
    if (!converted_point.has_value()) {
      LOG(ERROR) << "Rail parsing failed, malformed point: " << point;
      return {};
    }
    result.push_back(converted_point.value());
  }
  return result;
}

std::vector<World::WorldSpaceCoordinates> LookUpSegmentPoints(const std::vector<World::WorldSpaceCoordinates>& points,
                                                              const nlohmann::json& point_ids) {
  if (point_ids.is_null()) {
    LOG(ERROR) << "Rail segment parsing failed, could not find 'points' entry";
    return {};
  }
  if (!point_ids.is_array()) {
    LOG(ERROR) << "Rail segment parsing failed, 'points' entry is not array but instead:" << point_ids;
    return {};
  }

  std::vector<World::WorldSpaceCoordinates> result;
  for (const auto& point_id : point_ids) {
    if (!point_id.is_number_integer()) {
      LOG(ERROR) << "Rail segment parsing failed, point ID is not an integer: " << point_id;
      return {};
    }
    const auto converted_point_id = point_id.get<unsigned>();
    if (points.size() <= converted_point_id) {
      LOG(ERROR) << "Rail segment parsing failed, point ID: " << point_id << "is out of bounds, max: " << points.size();
      return {};
    }
    result.push_back(points[converted_point_id]);
  }
  return result;
}

std::vector<Rails::SegmentEndpointId> ParseConnections(const nlohmann::json& connections, const std::string& key) {
  const auto& connection_segments = connections[key];
  if (connection_segments.is_null()) {
    // Handle null silently, this is an optional entry
    return {};
  }
  if (!connection_segments.is_array()) {
    LOG(ERROR) << "Rail segment parsing failed, 'connections." << key
               << "' is not array but instead:" << connection_segments;
    return {};
  }
  std::vector<Rails::SegmentEndpointId> result;
  for (const auto& segment : connection_segments) {
    const auto& segment_id = segment["id"];
    if (segment_id.is_null()) {
      LOG(ERROR) << "Rail segment parsing failed, 'connections." << key
                 << "' has connection that does not contain an ID: " << segment;
      return {};
    }
    if (!segment_id.is_number_integer()) {
      LOG(ERROR) << "Rail segment parsing failed, 'connections." << key
                 << "' contains malformed segment ID that is not an integer: " << segment_id;
      return {};
    }
    const auto& segment_point = segment.contains("point") ? segment["point"] : nlohmann::json();
    auto result_segment_point = Rails::SegmentEndpoint::kEnd;
    if (!segment_point.is_null()) {
      if (!segment_point.is_string()) {
        LOG(ERROR) << "Rail segment parsing failed, 'connections." << key
                   << "' contains malformed segment connection point that is not a string: " << segment_point;
        return {};
      }
      if (!segment_point.is_string()) {
        LOG(ERROR) << "Rail segment parsing failed, 'connections." << key
                   << "' contains malformed segment connection point that is not a string: " << segment_point;
      }
      const auto segment_point_string = segment_point.get<std::string>();
      if (segment_point_string != "begin" && segment_point_string != "end") {
        LOG(ERROR) << "Rail segment parsing failed, 'connections." << key
                   << "' contains malformed segment connection point. expected 'begin' or 'end', got: "
                   << segment_point_string;
      }
      if (segment_point_string == "begin") {
        result_segment_point = Rails::SegmentEndpoint::kBegin;
      }
    }
    result.push_back({segment_id.get<unsigned>(), result_segment_point});
  }
  return result;
}

std::pair<std::vector<Rails::SegmentEndpointId>, std::vector<Rails::SegmentEndpointId>> ParseConnections(
    const nlohmann::json& connections) {
  if (connections.is_null()) {
    return {};
  }
  return {ParseConnections(connections, "begin"), ParseConnections(connections, "end")};
}

}  // namespace

std::unique_ptr<Rails> RailParser::Parse(const nlohmann::json& data) {
  auto rails = std::make_unique<Rails>();

  const auto curve_points = ParsePoints(data["points"]);
  if (curve_points.empty()) {
    // Parsing failed, quit early and return empty rails instance
    return rails;
  }

  // parse segments
  for (auto const [index, segment] : data["segments"] | std::views::enumerate) {
    const auto segment_points = LookUpSegmentPoints(curve_points, segment["points"]);
    const auto [begin_connections, end_connections] = ParseConnections(segment["connections"]);
    rails->AddSegment({static_cast<unsigned>(index)}, segment_points, begin_connections, end_connections);
  }

  return rails;
}

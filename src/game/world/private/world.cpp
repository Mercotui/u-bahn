// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/world/world.h"

#include <absl/log/log.h>
#include <mp-units/systems/si/si.h>

#include <memory>
#include <vector>

#include "game/world/rails.h"
#include "mercotui/ubahn/world/v1/world.pb.h"

namespace {
using google::protobuf::RepeatedPtrField;
using mercotui::ubahn::world::v1::Connection;
using mercotui::ubahn::world::v1::CONNECTION_POINT_BEGIN;
using mercotui::ubahn::world::v1::ConnectionPoint;
using mercotui::ubahn::world::v1::Point;
using mercotui::ubahn::world::v1::Section;
using mercotui::ubahn::world::v1::SegmentId;
using mp_units::si::metre;

/**
 * Get the SegmentID
 * @param segment_id SegmentId protobuf message
 * @return The segment ID, can be null segment ID if the message value was not set
 */
auto GetId(const SegmentId& segment_id) { return Rails::SegmentId{segment_id.value()}; }

/**
 * Get map the protobuf enum to the Rails::SegmentEndpoint enum
 * @param connection_point the protobuf connection point value
 * @return The corresponding SegmentEndpoint value, kEnd if the protobuf enum value is invalid
 */
auto GetConnectionPoint(const ConnectionPoint connection_point) {
  return connection_point == CONNECTION_POINT_BEGIN ? Rails::SegmentEndpoint::kBegin : Rails::SegmentEndpoint::kEnd;
}

/**
 * Convert curve points to world space coordinates
 * @param points A repeated protobuf point message
 * @return a vector of WorldSpaceCoordinates
 */
auto GetCurve(const RepeatedPtrField<Point>& points) {
  std::vector<World::WorldSpaceCoordinates> result;
  for (const auto& point : points) {
    result.emplace_back(World::origin + point.x() * metre, World::origin + point.y() * metre,
                        World::origin + point.z() * metre);
  }
  return result;
}

/**
 * Convert the repeated protobuf Connection message into a vector of SegmentEndpointIds
 * @param connections the protobuf connection messages
 * @return the mapped vector of SegmentEndpoints
 */
auto GetConnections(const RepeatedPtrField<Connection>& connections) {
  std::vector<Rails::SegmentEndpointId> result;
  for (const auto& connection : connections) {
    result.emplace_back(GetId(connection.id()), GetConnectionPoint(connection.connection_point()));
  }
  return result;
}
}  // namespace

std::unique_ptr<Rails> World::Load(std::istream* data) {
  auto result = std::make_unique<Rails>();

  Section section;
  if (!section.ParseFromIstream(data)) {
    LOG(ERROR) << "Failed to parse section data, invalid format";
    return {};
  }

  if (!section.has_rails()) {
    LOG(ERROR) << "Failed to parse section data, section does not contain rails";
    return {};
  }

  for (const auto& segment : section.rails().segments()) {
    result->AddSegment(GetId(segment.id()), GetCurve(segment.points()),
                       GetConnections(segment.connections_at_begin_point()),
                       GetConnections(segment.connections_at_end_point()));
  }
  return result;
}

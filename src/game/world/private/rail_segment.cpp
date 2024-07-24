#include "game/world/private/rail_segment.h"

#include <absl/log/log.h>
#include <raylib.h>

namespace {
using mp_units::si::metre;

double ToNumericalFromOrigin(const World::Coordinate coordinate) {
  return coordinate.quantity_from(World::origin).numerical_value_in(metre);
}

std::vector<bezier::Point> ToBezierPoints(const std::vector<World::WorldSpaceCoordinates>& points) {
  std::vector<bezier::Point> result;
  for (const auto& point : points) {
    double x = ToNumericalFromOrigin(point.x);
    double y = ToNumericalFromOrigin(point.y);
    result.emplace_back(x, y);
  }
  return result;
}

Vector3 ToRaylibVector3(const bezier::Point& point) {
  return {.x = static_cast<float>(point.x), .y = static_cast<float>(point.y), .z = 0.0f};
}

Rails::SegmentTraverseDirection DetermineNextTraverseDirection(
    Rails::SegmentTraverseDirection current_direction, Rails::SegmentEndpoint connection_point_of_current_segment,
    Rails::SegmentEndpoint connection_point_of_next_segment) {
  using Rails::SegmentTraverseDirection::kBackward;
  using Rails::SegmentTraverseDirection::kForward;

  if (connection_point_of_current_segment != connection_point_of_next_segment) {
    // This is a beginning to end connection, maintain current traversal direction
    return current_direction;
  }

  // This is a beginning to beginning or end to end connection, invert traversal direction
  return current_direction == kForward ? kBackward : kForward;
}

constexpr unsigned kDebugDrawSampleCount{5};
constexpr float kDebugDrawSphereSize{0.2f};
constexpr Color kDebugDrawSampleColor{BLUE};
constexpr Color kDebugDrawControlColor{PURPLE};
}  // namespace

RailSegment::RailSegment(const std::vector<World::WorldSpaceCoordinates>& curve_points) {
  const auto curve_points_2d = ToBezierPoints(curve_points);
  const auto point_count = curve_points_2d.size();
  switch (point_count) {
    default: {
      LOG(ERROR) << "Rail segment curve has too many points, maximum is 4, actual is " << point_count;
      return;
    }
    case 0:
      [[fallthrough]];
    case 1: {
      LOG(ERROR) << "Rail segment curve has too few points, minimum is 2, actual is " << point_count;
      return;
    }
    case 2: {
      curve_ = bezier::Bezier<1>(curve_points_2d);
      break;
    }
    case 3: {
      curve_ = bezier::Bezier<2>(curve_points_2d);
      break;
    }
    case 4: {
      curve_ = bezier::Bezier<3>(curve_points_2d);
      break;
    }
  }

  curve_length_ = std::visit([](auto&& arg) -> Units::Distance { return arg.length() * metre; }, curve_);
}

RailSegment::~RailSegment() = default;

World::WorldSpaceCoordinates RailSegment::WorldSpace(const Units::Distance location_in_segment) const {
  double t = (location_in_segment / curve_length_).numerical_value_in(mp_units::one);
  t = std::clamp(t, 0.0, 1.0);
  auto point = std::visit([t](auto&& arg) -> bezier::Point { return arg.valueAt(t); }, curve_);
  return {.x = World::origin + point.x * metre, .y = World::origin + point.y * metre, .z = height_};
}

RailSegment::TraverseResult RailSegment::Traverse(const Rails::Location location,
                                                  const Units::Distance distance) const {
  // The train might be oriented backwards on the track
  const double direction_correction =
      location.intra_segment_direction == Rails::SegmentTraverseDirection::kForward ? 1.0 : -1.0;
  const Units::Distance intra_segment_distance = distance * direction_correction;

  // Traverse requested distance with regard to the  direction we are facing
  const Units::Distance new_intra_segment_location = location.intra_segment_location + intra_segment_distance;

  if (0.0 * metre <= new_intra_segment_location && new_intra_segment_location <= curve_length_) {
    // We completed the requested distance, nothing left to do
    return TraverseCompletionResult{.segment = location.segment,
                                    .intra_segment_location = new_intra_segment_location,
                                    .intra_segment_direction = location.intra_segment_direction};
  }

  // The segment ended before we could traverse the requested distance, try to continue in next segment.
  auto connection_point = Rails::SegmentEndpoint::kBegin;
  Units::Distance remainder = new_intra_segment_location;
  Units::Distance dead_end = 0.0 * metre;
  if (new_intra_segment_location > curve_length_) {
    connection_point = Rails::SegmentEndpoint::kEnd;
    remainder = new_intra_segment_location - curve_length_;
    dead_end = curve_length_;
  }
  if (const auto next_segment = DetermineNext(connection_point)) {
    return TraverseIncompleteResult{
        .next_segment = next_segment,
        .direction_in_next_segment =
            DetermineNextTraverseDirection(location.intra_segment_direction, connection_point, next_segment.end_point),
        .remainder = remainder * direction_correction,
    };
  }

  // We reached a dead end, traversal is complete even though we did not reach the desired distance.
  return TraverseCompletionResult{.segment = location.segment,
                                  .intra_segment_location = dead_end,
                                  .intra_segment_direction = location.intra_segment_direction};
}

Rails::SegmentEndpointId RailSegment::DetermineNext(const Rails::SegmentEndpoint connection_point) const {
  if (connection_point == Rails::SegmentEndpoint::kBegin) {
    if (previous_switch) {
      return previous_diverging;
    }
    return previous;
  }

  if (next_switch) {
    return next_diverging;
  }
  return next;
}

void RailSegment::DrawDebug() {
  std::visit(
      [](auto&& arg) {
        // Draw curve's control points
        for (int i = 0; i < arg.size(); i++) {
          auto current_point = ToRaylibVector3(arg[i]);
          DrawSphere(current_point, kDebugDrawSphereSize, kDebugDrawControlColor);
        }

        for (int i = 1; i < kDebugDrawSampleCount; i++) {
          float t = static_cast<float>(i) / kDebugDrawSampleCount;
          auto point = ToRaylibVector3(arg.valueAt(t));
          DrawSphere(point, kDebugDrawSphereSize, kDebugDrawSampleColor);
        }
      },
      curve_);
}

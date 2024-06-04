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
  const Units::Distance new_intra_segment_location = location.intra_segment_location + distance;
  if (!(0.0 * metre > new_intra_segment_location || new_intra_segment_location > curve_length_)) {
    return TraverseCompletionResult(location.segment, new_intra_segment_location);
  }

  // TODO(Menno 19.05.2024) For now I'm assuming we travel backwards in the previous segment, but this is not
  //  necessarily true, for example turning-loops might start and end at the same point with inverted direction.
  auto travel_direction = TraverseDirection::kBackward;
  Units::Distance remainder = new_intra_segment_location;
  Units::Distance dead_end = 0.0 * metre;
  if (new_intra_segment_location > curve_length_) {
    // TODO(Menno 19.05.2024) For now I'm assuming we travel forward in the next segment, same as above
    travel_direction = TraverseDirection::kForward;
    remainder = new_intra_segment_location - curve_length_;
    dead_end = curve_length_;
  }
  if (const auto next_segment = DetermineNext(travel_direction)) {
    return TraverseIncompleteResult{
        .next_segment = next_segment, .direction_in_next_segment = travel_direction, .remainder = remainder};
  }
  // We reached a dead end, traversal is complete even though we did not reach the desired distance.
  return TraverseCompletionResult(location.segment, dead_end);
}
Rails::SegmentId RailSegment::DetermineNext(const RailSegment::TraverseDirection direction) const {
  // TODO(Menno 19.05.2024) implement switching here
  if (direction == TraverseDirection::kBackward) {
    return previous;
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

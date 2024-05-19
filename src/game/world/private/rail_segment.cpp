#include "game/world/private/rail_segment.h"

#include <absl/log/log.h>

namespace {
std::vector<bezier::Point> WorldSpaceCoordinatesTo2dPoints(const std::vector<World::WorldSpaceCoordinates>& points) {
  std::vector<bezier::Point> result;
  for (const auto& point : points) {
    result.emplace_back(point.x, point.y);
  }
  return result;
}
}  // namespace

RailSegment::RailSegment(const std::vector<World::WorldSpaceCoordinates>& curve_points) {
  const auto curve_points_2d = WorldSpaceCoordinatesTo2dPoints(curve_points);
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

  curve_length_ = std::visit([](auto&& arg) -> float { return arg.length(); }, curve_);
}

RailSegment::~RailSegment() = default;

World::WorldSpaceCoordinates RailSegment::WorldSpace(const float location_in_segment) const {
  float location_in_curve = std::clamp(location_in_segment / curve_length_, 0.0f, 1.0f);
  auto point =
      std::visit([location_in_curve](auto&& arg) -> bezier::Point { return arg.valueAt(location_in_curve); }, curve_);
  return {.x = static_cast<float>(point.x), .y = static_cast<float>(point.y), .z = height_};
}

RailSegment::TraverseResult RailSegment::Traverse(const Rails::Location location, const float distance) const {
  const float new_intra_segment_location = location.intra_segment_location + distance;
  if (!(0.0f > new_intra_segment_location || new_intra_segment_location > curve_length_)) {
    return TraverseCompletionResult(location.segment, new_intra_segment_location);
  }

  // TODO(Menno 19.05.2024) For now I'm assuming we travel backwards in the previous segment, but this is not
  //  necessarily true, for example turning-loops might start and end at the same point with inverted direction.
  auto travel_direction = TraverseDirection::kBackward;
  float remainder = new_intra_segment_location;
  float dead_end = 0.0f;
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

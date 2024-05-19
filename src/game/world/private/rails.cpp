#include "game/world/rails.h"

#include <absl/log/log.h>

#include <memory>
#include <variant>

#include "game/world/private/rail_segment.h"
#include "game/world/world.h"

namespace {
//! This is a massive maximum traversal limit, just to help debugging in case of an infinite loop.
constexpr unsigned kMaxTraverseIterations{10000};

Rails::Location LocationFromIncompleteTraverseResult(
    const Rails::SegmentMap& segments, const RailSegment::TraverseIncompleteResult& incomplete_traverse_result) {
  Rails::Location location{.segment = incomplete_traverse_result.next_segment, .intra_segment_location = 0.0f};

  if (incomplete_traverse_result.direction_in_next_segment == RailSegment::TraverseDirection::kForward) {
    return location;
  }

  const auto segment_it = segments.find(location.segment);
  if (segment_it == segments.end()) {
    LOG(ERROR) << "Trying to traverse across unknown rail segment " << location.segment.id;
    return location;
  }

  location.intra_segment_location = segment_it->second->Length();

  return location;
}

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
}  // namespace

// Constructor and destructor need to be defined here for forwards declaration purposes
Rails::Rails() = default;
Rails::~Rails() = default;

void Rails::AddSegment(SegmentId id, const std::vector<World::WorldSpaceCoordinates>& curve_points,
                       Rails::SegmentId incoming, Rails::SegmentId outgoing, Rails::SegmentId incoming_switch,
                       bool incoming_switch_selected, Rails::SegmentId outgoing_switch, bool outgoing_switch_selected) {
  auto segment = std::make_unique<RailSegment>(curve_points);
  segment->previous = incoming;
  segment->previous_diverging = incoming_switch;
  segment->previous_switch = incoming_switch_selected;

  segment->next = outgoing;
  segment->next_diverging = outgoing_switch;
  segment->next_switch = outgoing_switch_selected;

  segments_.emplace(id, std::move(segment));
}

World::WorldSpaceCoordinates Rails::WorldSpace(const Rails::Location& location) const {
  const auto& segment = segments_.find(location.segment);
  if (segment == segments_.end()) {
    LOG(ERROR) << "Trying to get WorldSpace for Location, but Rail segment " << location.segment.id << " is unknown";
    return {};
  }
  return segment->second->WorldSpace(location.intra_segment_location);
}

Rails::Location Rails::Traverse(const Rails::Location& initial_location, const float requested_distance) const {
  float remainder = requested_distance;
  Location location = initial_location;

  unsigned iterations{0};
  while (remainder != 0.0f) {
    if (kMaxTraverseIterations < iterations++) {
      LOG(ERROR) << "Giving up on traversal after " << kMaxTraverseIterations
                 << " iterations.\ninitial_location{ segment=" << initial_location.segment.id
                 << ", intra_segment_location=" << initial_location.intra_segment_location
                 << "}\n requested_distance=" << requested_distance
                 << "\n location{ segment=" << initial_location.segment.id
                 << ", intra_segment_location=" << initial_location.intra_segment_location << "}";
      return initial_location;
    }
    const auto segment_it = segments_.find(location.segment);
    if (segment_it == segments_.end()) {
      LOG(ERROR) << "Trying to traverse across unknown rail segment " << location.segment.id;
      return initial_location;
    }
    const auto& segment = segment_it->second;

    if (auto traverse_result = segment->Traverse(location, remainder);
        std::holds_alternative<RailSegment::TraverseIncompleteResult>(traverse_result)) {
      auto incomplete_traverse_result = std::get<RailSegment::TraverseIncompleteResult>(traverse_result);
      remainder = incomplete_traverse_result.remainder;
      location = LocationFromIncompleteTraverseResult(segments_, incomplete_traverse_result);
    } else if (std::holds_alternative<RailSegment::TraverseCompletionResult>(traverse_result)) {
      location = std::get<RailSegment::TraverseCompletionResult>(traverse_result);
      remainder = 0.0f;
    } else {
      LOG(ERROR) << "Unknown result from traversing rail segment";
      return initial_location;
    }
  }
  return location;
}

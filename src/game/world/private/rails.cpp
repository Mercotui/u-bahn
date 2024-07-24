#include "game/world/rails.h"

#include <absl/log/log.h>

#include <memory>
#include <variant>

#include "game/world/private/rail_segment.h"
#include "game/world/world.h"

namespace {
using mp_units::si::metre;

//! This is a massive maximum traversal limit, just to help debugging in case of an infinite loop.
constexpr unsigned kMaxTraverseIterations{10000};

Rails::Location LocationFromIncompleteTraverseResult(
    const Rails::SegmentMap& segments, const RailSegment::TraverseIncompleteResult& incomplete_traverse_result) {
  Rails::Location location{.segment = incomplete_traverse_result.next_segment.id,
                           .intra_segment_location = 0.0 * metre,
                           .intra_segment_direction = incomplete_traverse_result.direction_in_next_segment};

  if (incomplete_traverse_result.next_segment.end_point == Rails::SegmentEndpoint::kBegin) {
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

void MakeConnection(Rails::SegmentMap::iterator segment_a_iterator, const Rails::SegmentEndpoint end_point_a,
                    const Rails::SegmentEndpointId& end_point_id_b) {
  const auto& segment_a_id = segment_a_iterator->first;
  auto& segment_a = segment_a_iterator->second;

  switch (end_point_a) {
    case Rails::SegmentEndpoint::kBegin: {
      if (segment_a->previous == end_point_id_b || segment_a->previous_diverging == end_point_id_b) {
        return;
      } else if (segment_a->previous.id == Rails::NullSegmentId) {
        segment_a->previous = end_point_id_b;
      } else if (segment_a->previous_diverging.id == Rails::NullSegmentId) {
        segment_a->previous_diverging = end_point_id_b;
      } else {
        LOG(WARNING) << "Segment(" << segment_a_id.id << ") is already at capacity of incoming connections";
      }
      break;
    }
    case Rails::SegmentEndpoint::kEnd: {
      if (segment_a->next == end_point_id_b || segment_a->next_diverging == end_point_id_b) {
        return;
      } else if (segment_a->next.id == Rails::NullSegmentId) {
        segment_a->next = end_point_id_b;
      } else if (segment_a->next_diverging.id == Rails::NullSegmentId) {
        segment_a->next_diverging = end_point_id_b;
      } else {
        LOG(WARNING) << "Segment(" << segment_a_id.id << ") is already at capacity of outgoing connections";
      }
      break;
    }
    default: {
      LOG(FATAL) << "Unknown SegmentEndpoint, value="
                 << static_cast<std::underlying_type<Rails::SegmentEndpoint>::type>(end_point_a);
      return;
    }
  }
}

bool MakeConnection(Rails::SegmentMap* segments, const Rails::SegmentEndpointId endpoint_id_a,
                    const Rails::SegmentEndpointId endpoint_id_b) {
  auto segment_a = segments->find(endpoint_id_a.id);
  if (segment_a == segments->end()) {
    LOG(INFO) << "Can not (yet) make connection between Segment A (" << endpoint_id_a.id.id << ") and Segment B ("
              << endpoint_id_b.id.id << "), unknown Segment A";
    return false;
  }
  auto segment_b = segments->find(endpoint_id_b.id);
  if (segment_b == segments->end()) {
    LOG(INFO) << "Can not (yet) make connection between Segment A (" << endpoint_id_a.id.id << ") and Segment B ("
              << endpoint_id_b.id.id << "), unknown Segment B";
    return false;
  }

  MakeConnection(segment_a, endpoint_id_a.end_point, endpoint_id_b);
  MakeConnection(segment_b, endpoint_id_b.end_point, endpoint_id_a);
  return true;
}
}  // namespace

// Constructor and destructor need to be defined here for forwards declaration purposes
Rails::Rails() = default;
Rails::~Rails() = default;

void Rails::AddSegment(SegmentId id, const std::vector<World::WorldSpaceCoordinates>& curve_points,
                       std::vector<SegmentEndpointId> incoming_segments,
                       std::vector<SegmentEndpointId> outgoing_segments) {
  auto segment = std::make_unique<RailSegment>(curve_points);
  if (incoming_segments.size() > 2) {
    LOG(ERROR) << "Ignoring invalid rail segment: too many incoming segments, max is 2, actual is: "
               << incoming_segments.size();
    return;
  }
  if (outgoing_segments.size() > 2) {
    LOG(ERROR) << "Ignoring invalid rail segment: too many outgoing segments, max is 2, actual is: "
               << outgoing_segments.size();
    return;
  }

  segments_.emplace(id, std::move(segment));

  const auto begin_point_id = SegmentEndpointId(id, SegmentEndpoint::kBegin);
  const auto end_point_id = SegmentEndpointId(id, SegmentEndpoint::kEnd);

  // We need to check the failed connections map, there could be a pending connection with a previously created segment.
  // Add any pending incoming connections to the incoming_segments list.
  auto pending_incoming_connections = failed_connections_.equal_range(begin_point_id);
  std::for_each(pending_incoming_connections.first, pending_incoming_connections.second,
                [id, &incoming_segments](const auto& entry) {
                  LOG(INFO) << "Found pending incoming connection " << entry.second.id.id << " for segment " << id.id;
                  incoming_segments.push_back(entry.second);
                });
  failed_connections_.erase(begin_point_id);

  // Add any pending outgoing connections to the outgoing_segments list.
  auto pending_outgoing_connections = failed_connections_.equal_range(end_point_id);
  std::for_each(pending_outgoing_connections.first, pending_outgoing_connections.second,
                [id, &outgoing_segments](const auto& entry) {
                  LOG(INFO) << "Found pending outgoing connection " << entry.second.id.id << " for segment " << id.id;
                  outgoing_segments.push_back(entry.second);
                });
  failed_connections_.erase(end_point_id);

  // The connections are filtered to keep only the highest priority entries
  incoming_segments.erase(unique(incoming_segments.begin(), incoming_segments.end()), incoming_segments.end());
  outgoing_segments.erase(unique(outgoing_segments.begin(), outgoing_segments.end()), outgoing_segments.end());

  // Make incoming connections
  for (auto& incoming : incoming_segments) {
    if (!MakeConnection(&segments_, begin_point_id, incoming)) {
      // We could not make the connection, this could be due to the incoming ID not existing yet.
      failed_connections_.emplace(incoming, begin_point_id);
    }
  }

  // Make outgoing connections
  for (auto& outgoing : outgoing_segments) {
    if (!MakeConnection(&segments_, end_point_id, outgoing)) {
      // We could not make the connection, this could be due to the outgoing ID not existing yet.
      failed_connections_.emplace(outgoing, end_point_id);
    }
  }
}

World::WorldSpaceCoordinates Rails::WorldSpace(const Rails::Location& location) const {
  const auto& segment = segments_.find(location.segment);
  if (segment == segments_.end()) {
    LOG(ERROR) << "Trying to get WorldSpace for Location, but Rail segment " << location.segment.id << " is unknown";
    return {};
  }
  return segment->second->WorldSpace(location.intra_segment_location);
}

Rails::Location Rails::Traverse(const Rails::Location& initial_location,
                                const Units::Distance requested_distance) const {
  Units::Distance remainder = requested_distance;
  Location location = initial_location;

  unsigned iterations{0};
  while (remainder != 0.0 * metre) {
    if (kMaxTraverseIterations < iterations++) {
      LOG(ERROR) << "Giving up on traversal after " << kMaxTraverseIterations
                 << " iterations.\n  initial_location{segment=" << initial_location.segment.id
                 << ", intra_segment_location=" << initial_location.intra_segment_location.numerical_value_in(metre)
                 << "}\n  requested_distance=" << requested_distance.numerical_value_in(metre)
                 << "\n  location{segment=" << initial_location.segment.id
                 << ", intra_segment_location=" << initial_location.intra_segment_location.numerical_value_in(metre)
                 << "}";
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
      remainder = 0.0 * metre;
    } else {
      LOG(ERROR) << "Unknown result from traversing rail segment";
      return initial_location;
    }
  }
  return location;
}

void Rails::DrawDebug() {
  for (const auto& segment : segments_) {
    segment.second->DrawDebug();
  }
}

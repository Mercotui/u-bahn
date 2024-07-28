#pragma once

#include <array>
#include <map>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "game/world/world.h"

//! Forward declared, defined in game/world/rail_segment.h
class RailSegment;

class Rails {
 public:
  /**
   * strong typed ID that specifies a RailSegment
   */
  struct SegmentId {
    unsigned id{};

    explicit operator bool() const { return id != 0; }
    [[nodiscard]] bool operator==(const SegmentId& other) const { return id == other.id; }
    [[nodiscard]] bool operator<(const SegmentId& other) const { return id < other.id; }
    struct Hasher {
      std::size_t operator()(const SegmentId& instance) const { return std::hash<unsigned>()(instance.id); }
    };
  };

  //! Reserved Segment ID for unspecified segments
  static constexpr SegmentId NullSegmentId{.id = 0};

  using SegmentMap = std::unordered_map<SegmentId, std::unique_ptr<RailSegment>, SegmentId::Hasher>;

  //! One of the two endpoints of any railsegment
  enum class SegmentEndpoint { kBegin, kEnd };

  enum class SegmentTraverseDirection { kForward, kBackward };

  //! One specific endpoint of a railsegement
  struct SegmentEndpointId {
    SegmentId id{NullSegmentId};
    SegmentEndpoint end_point{SegmentEndpoint::kEnd};

    explicit operator bool() const { return id.operator bool(); }

    [[nodiscard]] bool operator==(const SegmentEndpointId& other) const {
      return id == other.id && end_point == other.end_point;
    }

    [[nodiscard]] bool operator<(const SegmentEndpointId& other) const {
      if (id < other.id) {
        return true;
      } else if (id == other.id) {
        return end_point < other.end_point;
      }
      return false;
    }
  };

  /**
   * A location on the rails
   */
  struct Location {
    SegmentId segment{NullSegmentId};
    Units::Distance intra_segment_location{};
    SegmentTraverseDirection intra_segment_direction{SegmentTraverseDirection::kForward};

    [[nodiscard]] bool operator==(const Location& other) const {
      return segment == other.segment && intra_segment_location == other.intra_segment_location;
    }
  };

  //! Constructor
  Rails();
  //! Destructor
  ~Rails();

  /**
   * Add rail segment with the given curve.
   * Incoming and outgoing switches will default to the first incoming and outgoing segment respectively.
   * @param id the unique ID of this rail segment
   * @param curve_points the curve of the RailSegment we want to build
   * @param incoming_segments the segments that lead into this segment, between 0 and 2 elements allowed
   * @param outgoing_segments the segments that lead out from this segment, between 0 and 2 elements allowed
   */
  void AddSegment(SegmentId id, const std::vector<World::WorldSpaceCoordinates>& curve_points,
                  std::vector<SegmentEndpointId> begin_connections = {},
                  std::vector<SegmentEndpointId> end_connections = {});

  /**
   * Get the world-space coordinates for this rail location.
   * @return world-space coordinates
   */
  [[nodiscard]] World::WorldSpaceCoordinates WorldSpace(const Location& location) const;

  /**
   * Traverse distance along the rails from starting point position.
   * @param initial_location
   * @param requested_distance
   * @return the new location after traversal, and a bool indicating whether or not the full distance was traversed
   */
  std::pair<Location, bool> Traverse(const Location& initial_location, Units::Distance requested_distance) const;

  /**
   * Draw the rail segments in debug mode.
   */
  void DrawDebug();

 private:
  SegmentMap segments_;
  std::multimap<SegmentEndpointId, SegmentEndpointId> failed_connections_;
};

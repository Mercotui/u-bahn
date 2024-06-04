#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

#include "game/world/rails.h"
#include "game/world/world.h"

//! Forward declared, defined in game/world/rail_segment.h
class RailSegment;

class Rails {
 public:
  //! Constructor
  Rails();
  //! Destructor
  ~Rails();

  /**
   * strong typed ID that specifies a RailSegment
   */
  struct SegmentId {
    unsigned id{};

    explicit operator bool() const { return id != 0; }
    [[nodiscard]] bool operator==(const SegmentId& other) const { return id == other.id; }
    struct Hasher {
      std::size_t operator()(const SegmentId& instance) const { return std::hash<unsigned>()(instance.id); }
    };
  };

  //! Reserved Segment ID for unspecified segments
  static constexpr SegmentId NullSegmentId{.id = 0};

  using SegmentMap = std::unordered_map<SegmentId, std::unique_ptr<RailSegment>, SegmentId::Hasher>;

  /**
   * A location on the rails
   */
  struct Location {
    SegmentId segment{NullSegmentId};
    Units::Distance intra_segment_location{};

    [[nodiscard]] bool operator==(const Location& other) const {
      return segment == other.segment && intra_segment_location == other.intra_segment_location;
    }
  };

  /**
   * Create a rail segment builder for a RailSegment with the given curve.
   * @param curve_points the curve of the RailSegment we want to build
   */
  void AddSegment(SegmentId id, const std::vector<World::WorldSpaceCoordinates>& curve_points,
                  Rails::SegmentId incoming = NullSegmentId, Rails::SegmentId outgoing = NullSegmentId,
                  Rails::SegmentId incoming_switch = NullSegmentId, bool incoming_switch_selected = false,
                  Rails::SegmentId outgoing_switch = NullSegmentId, bool outgoing_switch_selected = false);

  /**
   * Get the world-space coordinates for this rail location.
   * @return world-space coordinates
   */
  [[nodiscard]] World::WorldSpaceCoordinates WorldSpace(const Location& location) const;

  /**
   * Traverse distance along the rails from starting point position.
   * @param initial_location
   * @param requested_distance
   * @return
   */
  Location Traverse(const Location& initial_location, Units::Distance requested_distance) const;

  /**
   * Draw the rail segments in debug mode.
   */
  void DrawDebug();

 private:
  SegmentMap segments_;
};

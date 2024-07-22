#pragma once

#include <bezier/bezier.h>

#include <variant>
#include <vector>

#include "game/world/rails.h"

class RailSegment {
 public:
  enum class TraverseDirection { kForward, kBackward };

  using TraverseCompletionResult = Rails::Location;
  struct TraverseIncompleteResult {
    Rails::SegmentEndpointId next_segment;
    TraverseDirection direction_in_next_segment{};
    Units::Distance remainder{};
  };
  using TraverseResult = std::variant<TraverseCompletionResult, TraverseIncompleteResult>;

  explicit RailSegment(const std::vector<World::WorldSpaceCoordinates>& curve_points);
  ~RailSegment();

  [[nodiscard]] World::WorldSpaceCoordinates WorldSpace(Units::Distance location_in_segment) const;

  [[nodiscard]] Units::Distance Length() const { return curve_length_; }

  [[nodiscard]] TraverseResult Traverse(Rails::Location location, Units::Distance distance) const;

  [[nodiscard]] Rails::SegmentEndpointId DetermineNext(TraverseDirection direction) const;

  void DrawDebug();

  bool previous_switch{false};
  Rails::SegmentEndpointId previous{};
  Rails::SegmentEndpointId previous_diverging{};

  bool next_switch{false};
  Rails::SegmentEndpointId next{};
  Rails::SegmentEndpointId next_diverging{};

 private:
  // TODO(Menno 19.05.2024) Add changeable height  to rail segments
  World::Coordinate height_{};
  Units::Distance curve_length_{};
  std::variant<bezier::Bezier<1>, bezier::Bezier<2>, bezier::Bezier<3>> curve_;
};

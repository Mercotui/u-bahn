// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <bezier/bezier.h>

#include <variant>
#include <vector>

#include "game/world/rails.h"

class RailSegment {
 public:
  enum class TraversalConclusion { kCompleted, kDeadEnd };

  struct TraverseCompletionResult {
    Rails::Location location;
    TraversalConclusion conclusion;
  };
  struct TraverseIncompleteResult {
    Rails::SegmentEndpointId next_segment;
    Rails::SegmentTraverseDirection direction_in_next_segment{};
    Units::Distance remainder{};
  };
  using TraverseResult = std::variant<TraverseCompletionResult, TraverseIncompleteResult>;

  explicit RailSegment(const std::vector<World::WorldSpaceCoordinates>& curve_points);
  ~RailSegment();

  [[nodiscard]] World::WorldSpaceCoordinates WorldSpace(Units::Distance location_in_segment) const;

  [[nodiscard]] Units::Distance Length() const { return curve_length_; }

  [[nodiscard]] TraverseResult Traverse(Rails::Location location, Units::Distance distance) const;

  [[nodiscard]] Rails::SegmentEndpointId DetermineNext(Rails::SegmentEndpoint connection_point) const;

  void DrawDebug() const;

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

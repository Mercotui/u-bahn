#include <benchmark/benchmark.h>

#include "game/world/rails.h"
#include "game/world/units.h"

namespace {
constexpr Units::Distance kTranslationDistance = 0.1 * mp_units::si::metre;
constexpr auto kRailScale{1 * mp_units::si::metre};
}  // namespace

static void BM_SomeFunction(benchmark::State& state) {  // NOLINT(runtime/references)
  Rails rails;
  // construct a curved segment through (0, 0), (0, 10), (10, 10) and ending at (10, 0)
  // and then a straight rail segment back between (0, 0) and (10, 0)
  World::WorldSpaceCoordinates point_1 = {.x = World::origin + 0.0 * kRailScale, .y = World::origin + 0.0 * kRailScale};
  World::WorldSpaceCoordinates point_2 = {.x = World::origin + 0.0 * kRailScale,
                                          .y = World::origin + 10.0 * kRailScale};
  World::WorldSpaceCoordinates point_3 = {.x = World::origin + 10.0 * kRailScale,
                                          .y = World::origin + 10.0 * kRailScale};
  World::WorldSpaceCoordinates point_4 = {.x = World::origin + 10.0 * kRailScale,
                                          .y = World::origin + 0.0 * kRailScale};
  constexpr Rails::SegmentId id_1 = {.id = 1};
  constexpr Rails::SegmentId id_2 = {.id = 2};
  rails.AddSegment(id_1, {point_1, point_2, point_3, point_4}, {{id_2}});
  rails.AddSegment(id_2, {point_4, point_1}, {{id_1}});

  Rails::Location initial_location{.segment = id_1, .intra_segment_location = {}};

  for (auto _ : state) {  // This code gets timed
    const auto [new_location, completed] = rails.Traverse(initial_location, kTranslationDistance);
    const auto point = rails.WorldSpace(new_location);
  }
}

// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();

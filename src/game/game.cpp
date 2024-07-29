#include "game/game.h"

#include <mp-units/format.h>
#include <raylib.h>

#include <format>
#include <vector>

#include "game/control/control_scheme_mapper.h"
#include "game/input/input_manager_interface.h"
#include "game/world/rails.h"
#include "game/world/train.h"
#include "game/world/units.h"
#include "platform/platform.h"

namespace {
using Control::TrainControls;
constexpr auto kRailScale{7 * mp_units::si::metre};

enum class UnitCircleQuadrant {
  kTopRight,
  kTopLeft,
  kBottomLeft,
  kBottomRight,
};
std::vector<World::WorldSpaceCoordinates> CircleQuadrantAtOffset(const UnitCircleQuadrant quadrant,
                                                                 const World::WorldSpaceCoordinates origin) {
  // construct a clockwise unit-circle
  World::WorldSpaceCoordinates rail_point_1{.x = origin.x + 1.0 * kRailScale, .y = origin.y + 0.0 * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_1_1{.x = origin.x + 1.0 * kRailScale,
                                                      .y = origin.y + -0.552284749831 * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_1_2{.x = origin.x + 0.552284749831 * kRailScale,
                                                      .y = origin.y + -1.0 * kRailScale};
  World::WorldSpaceCoordinates rail_point_2{.x = origin.x + 0.0 * kRailScale, .y = origin.y + -1.0 * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_2_1{.x = origin.x + -0.552284749831 * kRailScale,
                                                      .y = origin.y + -1.0 * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_2_2{.x = origin.x + -1.0f * kRailScale,
                                                      .y = origin.y + -0.552284749831 * kRailScale};
  World::WorldSpaceCoordinates rail_point_3{.x = origin.x + -1.0 * kRailScale, .y = origin.y + 0.0 * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_3_1{.x = origin.x + -1.0f * kRailScale,
                                                      .y = origin.y + 0.552284749831 * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_3_2{.x = origin.x + -0.552284749831 * kRailScale,
                                                      .y = origin.y + 1.0 * kRailScale};
  World::WorldSpaceCoordinates rail_point_4{.x = origin.x + 0.0 * kRailScale, .y = origin.y + 1.0 * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_4_1{.x = origin.x + 0.552284749831 * kRailScale,
                                                      .y = origin.y + 1.0 * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_4_2{.x = origin.x + 1.0 * kRailScale,
                                                      .y = origin.y + 0.552284749831 * kRailScale};

  std::vector<World::WorldSpaceCoordinates> curve_points;

  switch (quadrant) {
    case UnitCircleQuadrant::kTopRight: {
      return {rail_point_1, rail_control_point_1_1, rail_control_point_1_2, rail_point_2};
    }
    case UnitCircleQuadrant::kTopLeft: {
      return {rail_point_2, rail_control_point_2_1, rail_control_point_2_2, rail_point_3};
    }
    case UnitCircleQuadrant::kBottomLeft: {
      return {rail_point_3, rail_control_point_3_1, rail_control_point_3_2, rail_point_4};
    }
    case UnitCircleQuadrant::kBottomRight: {
      return {rail_point_4, rail_control_point_4_1, rail_control_point_4_2, rail_point_1};
    }
    default: {
      return {};
    }
  }
}
}  // namespace

Game::Game()
    : input_(InputManagerFactory::Create(Platform::GetPlatform())),
      controls_mapper_(std::make_unique<ControlSchemeMapper>()),
      rails_(std::make_unique<Rails>()) {
  camera_ = {0};
  camera_.position = (::Vector3){-10.0f, -10.0f, 10.0f};
  camera_.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera_.up = (Vector3){0.0f, 0.0f, 1.0f};
  camera_.fovy = 45.0f;
  camera_.projection = CAMERA_PERSPECTIVE;

  SetTargetFPS(60);

  Rails::SegmentId id_1 = {1};
  Rails::SegmentId id_2 = {2};
  Rails::SegmentId id_3 = {3};
  Rails::SegmentId id_4 = {4};
  Rails::SegmentId id_5 = {5};
  Rails::SegmentId id_6 = {6};
  Rails::SegmentId id_7 = {7};

  // Circle
  rails_->AddSegment(id_1, CircleQuadrantAtOffset(UnitCircleQuadrant::kTopRight, {}),
                     {{id_4}, {id_5, Rails::SegmentEndpoint::kBegin}}, {{id_2, Rails::SegmentEndpoint::kBegin}});
  rails_->AddSegment(id_2, CircleQuadrantAtOffset(UnitCircleQuadrant::kTopLeft, {}), {{id_1}},
                     {{id_3, Rails::SegmentEndpoint::kBegin}});
  rails_->AddSegment(id_3, CircleQuadrantAtOffset(UnitCircleQuadrant::kBottomLeft, {}), {{id_2}},
                     {{id_4, Rails::SegmentEndpoint::kBegin}});
  //  rails_->AddSegment(id_4, CircleQuadrantAtOffset(UnitCircleQuadrant::kBottomRight, {}), {{id_3}},
  //                     {{id_1, Rails::SegmentEndpoint::kBegin}});

  // Turning Loop
  rails_->AddSegment(id_5,
                     CircleQuadrantAtOffset(UnitCircleQuadrant::kBottomLeft,
                                            {World::origin + 2 * kRailScale, World::origin + 0 * kRailScale}),
                     {{id_1, Rails::SegmentEndpoint::kBegin}}, {{id_7, Rails::SegmentEndpoint::kBegin}});
  rails_->AddSegment(id_6,
                     {{World::origin + 0 * kRailScale, World::origin + 1 * kRailScale},
                      {World::origin + 2 * kRailScale, World::origin + 1 * kRailScale}},
                     {{id_3}}, {{id_7, Rails::SegmentEndpoint::kBegin}});
  rails_->AddSegment(id_7,
                     CircleQuadrantAtOffset(UnitCircleQuadrant::kBottomRight,
                                            {World::origin + 2 * kRailScale, World::origin + 0 * kRailScale}),
                     {{id_6}, {id_5}});

  train_ = std::make_unique<Train>(
      *rails_, Rails::Location{.segment = id_7, .intra_segment_direction = Rails::SegmentTraverseDirection::kBackward},
      3);
}

Game::~Game() = default;

bool Game::Loop() {
  auto controls_variant = controls_mapper_->Map(input_->Poll(), Control::Mode::kTrain);
  Units::TimeDelta time = GetFrameTime() * mp_units::si::second;
  const auto controls = get<TrainControls>(controls_variant);
  if (controls.show_debug) {
    show_debug_ = !show_debug_;
  }
  train_->Control(controls, time);

  BeginDrawing();
  ClearBackground(RAYWHITE);

  BeginMode3D(camera_);
  train_->Draw();
  if (show_debug_) {
    rails_->DrawDebug();
    train_->DrawDebug();
  }
  EndMode3D();

  DrawText(std::format("Velocity={:.1f} km/h",
                       train_->Speed().numerical_value_in(mp_units::si::kilo<mp_units::si::metre> / mp_units::si::hour))
               .c_str(),
           40, 40, 20, BLACK);
  EndDrawing();

  return !WindowShouldClose();
}

#include "game/game.h"

#include <mp-units/format.h>
#include <raylib.h>

#include <format>

#include "game/control/control_scheme_mapper.h"
#include "game/input/input_manager_interface.h"
#include "game/world/rails.h"
#include "game/world/train.h"
#include "game/world/units.h"
#include "platform/platform.h"

namespace {
using Control::TrainControls;
constexpr float kRailScale{5};
}  // namespace

Game::Game()
    : input_(InputManagerFactory::Create(Platform::GetPlatform())),
      controls_mapper_(std::make_unique<ControlSchemeMapper>()),
      rails_(std::make_unique<Rails>()) {
  camera_ = {0};
  camera_.position = (::Vector3){10.0f, 10.0f, 10.0f};
  camera_.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera_.up = (Vector3){0.0f, 0.0f, 1.0f};
  camera_.fovy = 45.0f;
  camera_.projection = CAMERA_PERSPECTIVE;

  SetTargetFPS(60);

  // construct a clockwise unit-circle
  World::WorldSpaceCoordinates rail_point_1{.x = 1.0f * kRailScale, .y = 0.0f * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_1_1{.x = 1.0f * kRailScale, .y = -0.552284749831f * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_1_2{.x = 0.552284749831f * kRailScale, .y = -1.0f * kRailScale};
  World::WorldSpaceCoordinates rail_point_2{.x = 0.0f * kRailScale, .y = -1.0f * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_2_1{.x = -0.552284749831f * kRailScale, .y = -1.0f * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_2_2{.x = -1.0f * kRailScale, .y = -0.552284749831f * kRailScale};
  World::WorldSpaceCoordinates rail_point_3{.x = -1.0f * kRailScale, .y = 0.0f * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_3_1{.x = -1.0f * kRailScale, .y = 0.552284749831f * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_3_2{.x = -0.552284749831f * kRailScale, .y = 1.0f * kRailScale};
  World::WorldSpaceCoordinates rail_point_4{.x = 0.0f * kRailScale, .y = 1.0f * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_4_1{.x = 0.552284749831f * kRailScale, .y = 1.0f * kRailScale};
  World::WorldSpaceCoordinates rail_control_point_4_2{.x = 1.0f * kRailScale, .y = 0.552284749831f * kRailScale};

  rails_->AddSegment({.id = 1}, {rail_point_1, rail_control_point_1_1, rail_control_point_1_2, rail_point_2}, {.id = 4},
                     {.id = 2});
  rails_->AddSegment({.id = 2}, {rail_point_2, rail_control_point_2_1, rail_control_point_2_2, rail_point_3}, {.id = 1},
                     {.id = 3});
  rails_->AddSegment({.id = 3}, {rail_point_3, rail_control_point_3_1, rail_control_point_3_2, rail_point_4}, {.id = 2},
                     {.id = 4});
  rails_->AddSegment({.id = 4}, {rail_point_4, rail_control_point_4_1, rail_control_point_4_2, rail_point_1}, {.id = 3},
                     {.id = 1});
  train_ = std::make_unique<Train>(*rails_, Rails::Location{.segment = {.id = 1}});
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
  }
  EndMode3D();

  DrawText(std::format("Velocity={:.1f} km/h",
                       train_->Speed().numerical_value_in(mp_units::si::kilo<mp_units::si::metre> / mp_units::si::hour))
               .c_str(),
           40, 40, 20, BLACK);
  EndDrawing();

  return !WindowShouldClose();
}

#include "game/game.h"

#include <raylib.h>

#include "game/control/control_scheme_mapper.h"
#include "game/input/input_manager_interface.h"
#include "platform/platform.h"

namespace {
using Control::TrainControls;
constexpr float kMaxTrainVelocity{1.0f};
constexpr float kMinTrainVelocity{-kMaxTrainVelocity};

}  // namespace

Game::Game()
    : input_(InputManagerFactory::Create(Platform::Type::kDesktop)),
      controls_mapper_(std::make_unique<ControlSchemeMapper>()) {
  camera_ = {0};
  camera_.position = (::Vector3){10.0f, 10.0f, 10.0f};
  camera_.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera_.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera_.fovy = 45.0f;
  camera_.projection = CAMERA_PERSPECTIVE;

  model_ = LoadModel("/home/menno/Workspace/u-bahn/resources/lowpoly_berlin_u-bahn/untitled.glb");

  SetTargetFPS(60);
}

Game::~Game() { UnloadModel(model_); }

bool Game::Loop() {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  BeginMode3D(camera_);

  auto controls_variant = controls_mapper_->Map(input_->Poll(), Control::Mode::kTrain);
  const auto controls = get<TrainControls>(controls_variant);
  train_velocity_ += 0.0005f * controls.throttle;
  train_velocity_ = std::clamp(train_velocity_, kMinTrainVelocity, kMaxTrainVelocity);

  train_location_ += train_velocity_;
  DrawModel(model_, {train_location_, 0.0f, 0.0f}, 0.1, YELLOW);

  EndMode3D();
  DrawText(std::format("{} value={}", active_input_, controls.throttle).c_str(), 40, 40, 20, BLACK);

  EndDrawing();

  return !WindowShouldClose();
}

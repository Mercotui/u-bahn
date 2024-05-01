#include "game/game.h"

#include <raylib.h>

#include "platform/platform.h"

Game::Game() : input_(InputManagerFactory::Create(Platform::Type::kDesktop)) {
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
  UpdateCamera(&camera_, CAMERA_FREE);

  BeginDrawing();

  ClearBackground(RAYWHITE);

  BeginMode3D(camera_);

  float val{0.0f};
  for (const auto& sample : input_->Poll()) {
    if (sample.info.type == Input::Type::kJoystick && sample.info.id == 1) {
      val = sample.axes[2].value;
    }
  }

  DrawModel(model_, {val, 0.0f, 0.0f}, 1.0, YELLOW);

  EndMode3D();
  EndDrawing();

  return !WindowShouldClose();
}

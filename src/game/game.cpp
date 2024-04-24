#include "game/game.h"

#include <raylib.h>

bool Game::Loop() {
  Camera3D camera = {0};
  camera.position = (::Vector3){10.0f, 10.0f, 10.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  BeginDrawing();

  ClearBackground(RAYWHITE);

  BeginMode3D(camera);
  DrawCube(Vector3{0}, 1.0, 1.0, 1.0, BLUE);

  EndMode3D();
  EndDrawing();

  return running_;
}

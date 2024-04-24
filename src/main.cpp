#include <raylib.h>

#include "game/game.h"

namespace {
constexpr auto kInitialScreenWidth = 800;
constexpr auto kInitialScreenHeight = 600;
}  // namespace

int main() {
  InitWindow(kInitialScreenWidth, kInitialScreenHeight, "U-Bahn");
  MaximizeWindow();

  Game game{};
  while (game.Loop()) {
  }

  CloseWindow();
  return 0;
}

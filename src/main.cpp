#include <absl/log/globals.h>
#include <absl/log/initialize.h>
#include <raylib.h>

#include "game/game.h"

namespace {
constexpr auto kInitialScreenWidth = 1300;
constexpr auto kInitialScreenHeight = 900;
}  // namespace

int main() {
  absl::InitializeLog();
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
  InitWindow(kInitialScreenWidth, kInitialScreenHeight, "U-Bahn");

  Game game{};
  while (game.Loop()) {
  }

  CloseWindow();
  return 0;
}

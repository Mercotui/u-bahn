#include <absl/log/globals.h>
#include <absl/log/initialize.h>
#include <raylib.h>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

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

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg([](void* game) { static_cast<Game*>(game)->Loop(); }, &game, 0, 1);
#else
  while (game.Loop()) {
  }
#endif

  CloseWindow();
  return 0;
}

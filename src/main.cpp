#include <absl/log/globals.h>
#include <absl/log/initialize.h>
#include <absl/log/log.h>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include <cstdio>

#include "game/game.h"
#include "third_party/raylib/raylib.h"

namespace {
constexpr auto kInitialScreenWidth = 1300;
constexpr auto kInitialScreenHeight = 900;

absl::LogSeverity RaylibToAbslLogLevel(const Raylib::TraceLogLevel raylib_log_level) {
  switch (raylib_log_level) {
    case Raylib::LOG_DEBUG:
      // absl doesn't go below info level unlike every other log library for some reason
      [[fallthrough]];
    case Raylib::LOG_INFO:
      return absl::LogSeverity::kInfo;
    case Raylib::LOG_WARNING:
      return absl::LogSeverity::kWarning;
    case Raylib::LOG_ERROR:
      return absl::LogSeverity::kError;
    case Raylib::LOG_FATAL:
      return absl::LogSeverity::kFatal;
    default:
      // failsafe I guess, if log level is unknown then we treat it as an error.
      return absl::LogSeverity::kError;
  }
}
}  // namespace

int main() {
  absl::InitializeLog();
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kWarning);

  // divert raylib logging into absl logging
  Raylib::SetTraceLogCallback(+[](int level, const char* text, va_list args) {
    constexpr auto kMaxLogLength = 2048;
    char expanded_text[kMaxLogLength];
    std::vsnprintf(expanded_text, kMaxLogLength, text, args);
    const auto absl_level = RaylibToAbslLogLevel(static_cast<Raylib::TraceLogLevel>(level));
    LOG(LEVEL(absl_level)) << "raylib: " << expanded_text;
  });

  Raylib::SetConfigFlags(Raylib::FLAG_WINDOW_RESIZABLE | Raylib::FLAG_WINDOW_HIGHDPI | Raylib::FLAG_MSAA_4X_HINT);
  Raylib::InitWindow(kInitialScreenWidth, kInitialScreenHeight, "U-Bahn");

  Game game{};

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg([](void* game) { static_cast<Game*>(game)->Loop(); }, &game, 0, 1);
#else
  while (game.Loop()) {
  }
#endif

  Raylib::CloseWindow();
  return 0;
}

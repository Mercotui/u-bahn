// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include <absl/log/globals.h>
#include <absl/log/initialize.h>
#include <absl/log/log.h>

#include "game/game.h"
#include "platform/platform.h"

struct GameObj final : Platform::MainLoop {
  bool operator()() override { return game.Loop(); }

 private:
  Game game;
};

int main(int, char**) {
  absl::InitializeLog();
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kWarning);

  const auto platform = Platform::Init();
  if (!platform) {
    LOG(FATAL) << "No platform";
  }

  GameObj game;
  platform->Loop(&game);
  return 0;
}

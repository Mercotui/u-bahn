// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include <absl/log/globals.h>
#include <absl/log/initialize.h>
#include <absl/log/log.h>

#include "platform/platform.h"
#include "third_party/hello_triangle/hello_triangle.h"

struct GameObj final : Platform::MainLoop {
  bool operator()() override {
    HelloTriangle::Draw();
    return true;
  }
};

int main(int, char**) {
  absl::InitializeLog();
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kWarning);

  const auto platform = Platform::Init();
  if (!platform) {
    LOG(FATAL) << "No platform";
  }
  if (!HelloTriangle::Init()) {
    LOG(FATAL) << "No renderer";
  }

  GameObj game;
  platform->Loop(&game);
  HelloTriangle::Shutdown();
  return 0;
}

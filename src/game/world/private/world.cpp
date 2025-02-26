// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/world/world.h"

#include <absl/log/log.h>

#include <memory>

#include "game/world/rails.h"

std::unique_ptr<Rails> World::Load(std::istream* data) {
  auto result = std::make_unique<Rails>();

  return result;
}

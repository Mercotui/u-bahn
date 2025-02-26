// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <memory>
#include <nlohmann/json.hpp>

//! Forward Declared, defined in game/world/rails.h
class Rails;

namespace RailParser {
std::unique_ptr<Rails> Parse(const nlohmann::json& data);
}

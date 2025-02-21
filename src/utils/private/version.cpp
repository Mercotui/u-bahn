// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "utils/version.h"

#include <string_view>

namespace {
const auto kVersion = UBAHN_VERSION;
}

std::string_view Utils::GetVersion() { return kVersion; }

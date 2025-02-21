// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

namespace Platform {
enum class Type { kAndroid, kDesktop, kWeb };

Type GetPlatform();
}  // namespace Platform

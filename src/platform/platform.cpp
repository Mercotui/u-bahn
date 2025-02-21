// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "platform/platform.h"

namespace {
#if defined(PLATFORM_DESKTOP)
constexpr Platform::Type kPlatformType = Platform::Type::kDesktop;
#elif defined(PLATFORM_ANDROID)
constexpr Platform::Type kPlatformType = Platform::Type::kAndroid;
#elif defined(PLATFORM_WEB)
constexpr Platform::Type kPlatformType = Platform::Type::kWeb;
#endif
}  // namespace

namespace Platform {
Type GetPlatform() { return kPlatformType; }
}  // namespace Platform

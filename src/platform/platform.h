#pragma once

namespace Platform {
enum class Type { kAndroid, kDesktop, kWeb };

constexpr Type GetPlatform();
}  // namespace Platform
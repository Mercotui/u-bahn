#pragma once

namespace Platform {
enum class Type { kAndroid, kDesktop, kWeb };

Type GetPlatform();
}  // namespace Platform

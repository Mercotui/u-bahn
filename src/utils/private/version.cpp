#include "utils/version.h"

#include <string_view>

namespace {
const auto kVersion = UBAHN_VERSION;
}

std::string_view Utils::GetVersion() { return kVersion; }

#pragma once

#include <string>

namespace Input {
enum class Type {
  kKeyboard,
  kMouse,
  kTouch,
  kJoystick,
  kGamepad,
};

struct Config {
  bool enabled{true};
};

struct Info {
  int id{};
  std::string name;
  Type type{};

  Config config{};
};
}  // namespace Input
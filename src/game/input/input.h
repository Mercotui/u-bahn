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

struct Axis {
  float value{0.0f};
};

struct Button {
  bool pressed{false};
};

struct Sample {
  Info info;
  std::vector<Button> buttons;
  std::vector<Axis> axes;
};

using Samples = std::vector<Sample>;
}  // namespace Input
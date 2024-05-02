#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>

struct Input {
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

  struct Axis {
    std::string name;
    float value{0.0f};
    bool active{false};
  };

  struct Button {
    std::string name;
    bool down{false};
    bool changed{false};
  };

  int id{};
  std::string name;
  Type type{};
  bool active{false};

  std::vector<Button> buttons;
  std::vector<Axis> axes;

  Config config{};
};

using InputList = std::vector<std::shared_ptr<Input>>;

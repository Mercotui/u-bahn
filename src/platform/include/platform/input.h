// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <array>
#include <chrono>
#include <string>
#include <utility>
#include <vector>

#include "platform/clock.h"

struct Input {
  enum class Type : size_t {
    kKeyboard = 0,
    kMouse,
    kTouch,
    kGamepad,
    kEnumTypeSize  //! Size of Type enum
  };

  struct Config {
    bool enabled{true};
  };

  struct Axis {
    struct Sample {
      Clock::time_point time_point{};
      float value{};
    };

    std::string name;
    float value{0.0f};
    std::pair<Sample, Sample> history{{}, {}};
    unsigned history_size{};
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

using Inputs = std::array<std::vector<Input>, std::to_underlying(Input::Type::kEnumTypeSize)>;

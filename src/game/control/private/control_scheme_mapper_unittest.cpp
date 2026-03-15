// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/control/control_scheme_mapper.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <vector>

#include "platform/keyboard_mouse.h"

namespace {
using KeyboardMouseInput::Key;

Input::Button& GetButton(Input* input, const Key key) { return input->buttons[std::to_underlying(key)]; }
}  // namespace

TEST(ControlSchemeMapperTest, MapTrainControlsKeyboard) {
  ControlSchemeMapper mapper;
  Inputs inputs;
  auto& input = inputs[std::to_underlying(Input::Type::kKeyboard)].emplace_back(
      1, "keyboard", Input::Type::kKeyboard, true,
      std::vector<Input::Button>(static_cast<unsigned>(Key::kEnumKeySize)));

  auto& w = GetButton(&input, Key::kW);
  auto& s = GetButton(&input, Key::kS);
  auto& r = GetButton(&input, Key::kR);

  // Nothing pressed
  const auto controls1 = mapper.MapGameControls(inputs);
  const auto train_controls1 = controls1.train_controls;
  ASSERT_EQ(train_controls1.throttle, 0.0f);
  ASSERT_EQ(train_controls1.brake, 0.0f);

  // Key W down
  w.down = true;
  const auto controls2 = mapper.MapGameControls(inputs);
  const auto train_controls2 = controls2.train_controls;
  ASSERT_EQ(train_controls2.throttle, 1.0f);
  ASSERT_EQ(train_controls2.brake, 0.0f);

  // Key W lifts, key S down
  w.down = false;
  s.down = true;
  const auto controls3 = mapper.MapGameControls(inputs);
  const auto train_controls3 = controls3.train_controls;
  ASSERT_EQ(train_controls3.throttle, 0.0f);
  ASSERT_EQ(train_controls3.brake, 1.0f);

  // key S lifts, key R down
  s.down = false;
  r.down = true;
  r.changed = true;
  const auto controls4 = mapper.MapGameControls(inputs);
  const auto train_controls4 = controls4.train_controls;
  ASSERT_EQ(train_controls4.throttle, 0.0f);
  ASSERT_EQ(train_controls4.brake, 0.0f);
  ASSERT_TRUE(train_controls4.reverse);

  // R still down but not changed
  r.changed = false;
  const auto controls5 = mapper.MapGameControls(inputs);
  const auto train_controls5 = controls5.train_controls;
  ASSERT_EQ(train_controls5.throttle, 0.0f);
  ASSERT_EQ(train_controls5.brake, 0.0f);
  ASSERT_FALSE(train_controls5.reverse);

  // Nothing down
  r.changed = false;
  const auto controls6 = mapper.MapGameControls(inputs);
  const auto train_controls6 = controls6.train_controls;
  ASSERT_EQ(train_controls6.throttle, 0.0f);
  ASSERT_EQ(train_controls6.brake, 0.0f);
  ASSERT_FALSE(train_controls6.reverse);
}

TEST(ControlSchemeMapperTest, MapTrainControlsTouch) {
  ControlSchemeMapper mapper;
  Inputs inputs;
  auto& input = inputs[std::to_underlying(Input::Type::kTouch)].emplace_back(1, "touch", Input::Type::kTouch, true);
  input.buttons = {Input::Button{"Touch 1", true, true}};
  input.axes = {Input::Axis{.name = "Touch Axis Horizontal", .value = 43.0f, .active = false},
                Input::Axis{.name = "Touch Axis Vertical", .value = 1000.0f, .active = false}};

  // Initial touch event
  const auto controls1 = mapper.MapGameControls(inputs);
  const auto train_controls1 = controls1.train_controls;
  ASSERT_EQ(train_controls1.throttle, 0.0f);
  ASSERT_EQ(train_controls1.brake, 0.0f);

  // Finger moves up towards top of screen, which has lower X coordinate
  input.axes[1].value = 500.0f;
  input.buttons[0].changed = false;
  const auto controls2 = mapper.MapGameControls(inputs);
  const auto train_controls2 = controls2.train_controls;
  ASSERT_EQ(train_controls2.throttle, 5.0f);
  ASSERT_EQ(train_controls2.brake, 0.0f);

  // Finger moves down towards bottom of screen, which has higher X coordinate
  input.axes[1].value = 1500.0f;
  input.buttons[0].changed = false;
  const auto controls3 = mapper.MapGameControls(inputs);
  const auto train_controls3 = controls3.train_controls;
  ASSERT_EQ(train_controls3.throttle, 0.0f);
  ASSERT_EQ(train_controls3.brake, 5.0f);

  // Finger back at starting position
  input.axes[1].value = 1000.0f;
  const auto controls4 = mapper.MapGameControls(inputs);
  const auto train_controls4 = controls4.train_controls;
  ASSERT_EQ(train_controls4.throttle, 0.0f);
  ASSERT_EQ(train_controls4.throttle, 0.0f);

  // Touch lift event, finger no longer touching screen
  input.axes[1].value = 1000.0f;
  input.buttons[0].down = false;
  input.buttons[0].changed = true;
  const auto controls5 = mapper.MapGameControls(inputs);
  const auto train_controls5 = controls5.train_controls;
  ASSERT_EQ(train_controls5.throttle, 0.0f);
  ASSERT_EQ(train_controls5.throttle, 0.0f);
}

// TODO(Menno 10.04.2026) This testcase doesn't make sense in its current iteration, it needs a review.
//  I think it should be expanded to fit all supported types of input. However at the moment only keyboard is supported,
//  so the test can not be meaningfully executed.
TEST(ControlSchemeMapperTest, MapTrainControlsActiveSwitching) {
  using CheckFunc = void (*)(Inputs*, ControlSchemeMapper*);

  const CheckFunc check_touch_event = +[](Inputs* inputs, ControlSchemeMapper* mapper) {
    auto& input = (*inputs)[std::to_underlying(Input::Type::kTouch)].front();
    // Finger makes contact
    input.active = true;
    input.buttons[0].changed = true;
    input.buttons[0].down = true;
    input.axes[1].value = 1000.0f;
    const auto controls1 = mapper->MapGameControls(*inputs);
    const auto train_controls1 = controls1.train_controls;
    ASSERT_EQ(train_controls1.throttle, 0.0f);

    // Finger moves up towards top of screen, which has lower X coordinate
    input.axes[1].value = 500.0f;
    input.buttons[0].changed = false;
    const auto controls2 = mapper->MapGameControls(*inputs);
    const auto train_controls2 = controls2.train_controls;
    ASSERT_EQ(train_controls2.throttle, 5.0f);

    // Reset
    input.active = false;
    input.buttons[0].changed = false;
    input.buttons[0].down = false;
  };

  const CheckFunc check_keyboard_event = +[](Inputs* inputs, ControlSchemeMapper* mapper) {
    auto& input = (*inputs)[std::to_underlying(Input::Type::kKeyboard)].front();
    auto& w = GetButton(&input, Key::kW);
    // Nothing pressed
    const auto controls1 = mapper->MapGameControls(*inputs);
    const auto train_controls1 = controls1.train_controls;
    ASSERT_EQ(train_controls1.throttle, 0.0f);

    // Key W down
    input.active = true;
    w.down = true;
    w.changed = true;
    const auto controls2 = mapper->MapGameControls(*inputs);
    const auto train_controls2 = controls2.train_controls;
    ASSERT_EQ(train_controls2.throttle, 1.0f);
    ASSERT_EQ(train_controls2.brake, 0.0f);

    // Reset
    input.active = false;
    w.down = false;
    w.changed = false;
  };

  ControlSchemeMapper mapper;

  Inputs inputs;
  auto& touch = inputs[std::to_underlying(Input::Type::kTouch)].emplace_back(1, "touch", Input::Type::kTouch, false);
  touch.buttons = {Input::Button{"Touch 1", false, false}};
  touch.axes = {Input::Axis{.name = "Touch Axis Horizontal", .value = 43.0f, .active = false},
                Input::Axis{.name = "Touch Axis Vertical", .value = 1000.0f, .active = false}};
  inputs[std::to_underlying(Input::Type::kKeyboard)].emplace_back(
      2, "keyboard", Input::Type::kKeyboard, false,
      std::vector<Input::Button>(static_cast<unsigned>(Key::kEnumKeySize)));

  std::vector checks = {check_touch_event, check_keyboard_event};
  do {
    for (const auto& check : checks) {
      check(&inputs, &mapper);
    }
  } while (std::ranges::next_permutation(checks.begin(), checks.end()).found);
}

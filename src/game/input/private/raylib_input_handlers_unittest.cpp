// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/input/private/raylib_input_handlers.h"

#include <gtest/gtest.h>

#include "game/input/private/input_unittest_helpers.h"

namespace {
using InputUnittestHelpers::CountInputName;
using InputUnittestHelpers::CountInputType;
}  // namespace

TEST(RaylibInputHandlersTest, KeyboardPoll) {
  RaylibInputHandlers::Keyboard handler;
  const auto list = handler.Poll();
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(CountInputType(list, Input::Type::kKeyboard), 1);
}

TEST(RaylibInputHandlersTest, KeyboardSetConfig) {
  RaylibInputHandlers::Keyboard handler;
  const auto list_before = handler.Poll();
  EXPECT_EQ(list_before.size(), 1);

  // This should have no effect
  handler.SetConfig(list_before.front()->id, {.enabled = false});

  const auto list_after = handler.Poll();
  EXPECT_EQ(list_after.size(), 1);
}

TEST(RaylibInputHandlersTest, MousePoll) {
  RaylibInputHandlers::Mouse handler;
  const auto list = handler.Poll();
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(CountInputType(list, Input::Type::kMouse), 1);
}

TEST(RaylibInputHandlersTest, MouseSetConfig) {
  RaylibInputHandlers::Mouse handler;
  const auto list_before = handler.Poll();
  EXPECT_EQ(list_before.size(), 1);

  // This should have no effect
  handler.SetConfig(list_before.front()->id, {.enabled = false});

  const auto list_after = handler.Poll();
  EXPECT_EQ(list_after.size(), 1);
}

TEST(RaylibInputHandlersTest, TouchPoll) {
  RaylibInputHandlers::Touch handler;
  const auto list = handler.Poll();
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(CountInputType(list, Input::Type::kTouch), 1);
}

TEST(RaylibInputHandlersTest, TouchSetConfig) {
  RaylibInputHandlers::Mouse handler;
  const auto list_before = handler.Poll();
  EXPECT_EQ(list_before.size(), 1);

  // This should have no effect
  handler.SetConfig(list_before.front()->id, {.enabled = false});

  const auto list_after = handler.Poll();
  EXPECT_EQ(list_after.size(), 1);
}

// SPDX-FileCopyrightText: 2026 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#include "platform/private/input_helpers.h"

#include <gtest/gtest.h>

#include <chrono>

#include "platform/clock.h"

namespace {
using Sample = Input::Axis::Sample;
using std::chrono_literals::operator""ms;
}  // namespace

TEST(InputHelpersTest, AxisActivityDetector) {
  Input::Axis axis{.name = "unittest axis"};
  const auto time_1 = Clock::now();
  const Sample sample_1{.time_point = time_1, .value = 0.556688};
  EXPECT_FALSE(InputAxisHelpers::Assign(&axis, sample_1));

  const auto time_2 = time_1 + 33ms;
  const Sample sample_2{.time_point = time_2, .value = 0.59795};
  EXPECT_TRUE(InputAxisHelpers::Assign(&axis, sample_2));
}

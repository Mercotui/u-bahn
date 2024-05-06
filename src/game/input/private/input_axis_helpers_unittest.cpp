#include "game/input/private/input_axis_helpers.h"

#include <gtest/gtest.h>

#include <chrono>

namespace {
using Sample = InputAxisHelpers::ActivityDetector::Sample;
using InputAxisHelpers::SampleTimePoint;
using std::chrono_literals::operator""ms;
}  // namespace

TEST(InputAxisHelpersTest, ActivityDetector) {
  InputAxisHelpers::ActivityDetector detector;
  SampleTimePoint time_1 = std::chrono::steady_clock::now();
  Sample sample_1{.time_point = time_1, .value = 0.556688};
  EXPECT_FALSE(detector.Detect(sample_1));

  SampleTimePoint time_2 = time_1 + 33ms;
  Sample sample_2{.time_point = time_2, .value = 0.59795};
  EXPECT_TRUE(detector.Detect(sample_2));
}

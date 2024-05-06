#pragma once

#include <chrono>
#include <string>
#include <utility>

#include "game/input/input.h"

namespace InputAxisHelpers {
Input::Axis MapToAxis();

float MapFromAxis(Input::Axis axis);

using SampleTimePoint = std::chrono::time_point<std::chrono::steady_clock>;
class ActivityDetector {
 public:
  struct Sample {
    SampleTimePoint time_point;
    float value{};
  };

  explicit ActivityDetector(std::string logging_prefix = "");

  /**
   * Detect activity from input samples
   * @param new_sample newest input sample
   * @return if activity has occurred
   */
  bool Detect(Sample new_sample);

 private:
  std::pair<Sample, Sample> samples_;
  const std::string logging_prefix_;
};
}  // namespace InputAxisHelpers

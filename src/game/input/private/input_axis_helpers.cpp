// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/input/private/input_axis_helpers.h"

#include <absl/log/log.h>

#include <chrono>
#include <cmath>
#include <string>
#include <utility>

namespace {
using std::chrono_literals::operator""s;
using Sample = InputAxisHelpers::ActivityDetector::Sample;

const auto ProgramStartTimePoint = std::chrono::steady_clock::now();
//! Defined in change of units per ms
constexpr float kActivityThreshold = 0.0006f;
constexpr bool kActivityDetectionLogging{false};

float EstimateDerivative(Sample old_sample, Sample new_sample) {
  const std::chrono::duration<float, std::milli> time_delta = new_sample.time_point - old_sample.time_point;
  float divider = time_delta.count();
  if (!std::isnormal(divider)) {
    return NAN;
  }
  return (new_sample.value - old_sample.value) / (divider);
}

}  // namespace

InputAxisHelpers::ActivityDetector::ActivityDetector(std::string logging_prefix)
    : samples_(std::make_pair(Sample(), Sample())), logging_prefix_(std::move(logging_prefix)) {}

bool InputAxisHelpers::ActivityDetector::Detect(const Sample new_sample) {
  if (samples_.second.time_point == new_sample.time_point) {
    // sometimes multiple samples are recorded for the same time point, we only keep the newest value.
    samples_.second.value = new_sample.value;
  } else {
    samples_.first = samples_.second;
    samples_.second = new_sample;
  }

  if ((samples_.second.time_point - samples_.first.time_point) > 1.0s) {
    // we ignore samples older than 1 second
    return false;
  }

  auto derivative = EstimateDerivative(samples_.first, samples_.second);
  if (!std::isnormal(derivative)) {
    return false;
  }
  bool is_active = kActivityThreshold < fabsf(derivative);

  if constexpr (kActivityDetectionLogging) {
    LOG(INFO) << logging_prefix_ << ","
              << std::chrono::duration_cast<std::chrono::milliseconds>(new_sample.time_point - ProgramStartTimePoint)
              << "," << new_sample.value << "," << derivative << "," << (is_active ? "1.0" : "0.0");
  }
  return is_active;
}

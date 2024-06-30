#include "game/script/private/script_binding.h"

#include <mp-units/math.h>
#include <raylib.h>

namespace {
bool IsInRadius(const ScriptBinding::Trigger& trigger, const World::WorldSpaceCoordinates& point) {
  using mp_units::pow;
  using mp_units::sqrt;
  using mp_units::si::metre;

  auto diff_x = point.x - trigger.center.x;
  auto diff_y = point.y - trigger.center.y;
  auto distance = sqrt(pow<2>(diff_x) + pow<2>(diff_y));
  return distance < trigger.radius;
}

bool IsInSpeedRange(const ScriptBinding::SpeedRange& speed_range, Units::Speed speed) {
  return speed_range.minimum <= speed && speed <= speed_range.maximum;
}

float fToNumericalFromOrigin(World::Coordinate coordinate) {
  using mp_units::si::metre;
  return static_cast<float>(coordinate.quantity_from(World::origin).numerical_value_in(metre));
}

Vector3 ToRaylibVector3(const World::WorldSpaceCoordinates& world_space_coordinates) {
  float x = fToNumericalFromOrigin(world_space_coordinates.x);
  float y = fToNumericalFromOrigin(world_space_coordinates.y);
  float z = fToNumericalFromOrigin(world_space_coordinates.z);
  return {.x = x, .y = y, .z = z};
}
}  // namespace

std::shared_ptr<ScriptBinding> ScriptBinding::Instance() {
  static std::weak_ptr<ScriptBinding> instance;
  static std::mutex m;

  m.lock();
  auto ret = instance.lock();
  if (!ret) {
    ret.reset(new ScriptBinding());
    instance = ret;
  }
  m.unlock();

  return ret;
}

void ScriptBinding::CheckTriggers(const Train& train) {
  for (auto& trigger : triggers_) {
    if (IsInRadius(trigger, train.WorldSpaceLocation()) && IsInSpeedRange(trigger.speed_range, train.Speed())) {
      // Detect rising edge
      if (!trigger.triggered) {
        trigger.triggered = true;
        trigger.on_trigger(true);
      }
    } else {
      // Detect falling edge
      if (trigger.triggered) {
        trigger.triggered = false;
        trigger.on_trigger(false);
      }
    }
  }
}
void ScriptBinding::AddTrigger(double point_x, double point_y, double radius, double speed_min, double speed_max,
                               sol::function callback) {
  triggers_.push_back({
      .center{.x = World::origin + point_x * Units::Distance::unit,
              .y = World::origin + point_y * Units::Distance::unit},
      .radius = radius * Units::Distance::unit,
      .speed_range{.minimum = speed_min * Units::Speed::unit, .maximum = speed_max * Units::Speed::unit},
      .on_trigger = callback,
  });
}

void ScriptBinding::DrawDebug() {
  for (const auto& trigger : triggers_) {
    using mp_units::si::metre;

    auto center = ToRaylibVector3(trigger.center);
    double radius = trigger.radius.numerical_value_in(metre);
    DrawSphere(center, radius, RED);
  }
}

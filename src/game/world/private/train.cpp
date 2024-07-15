#include "game/world/train.h"

#include <raylib.h>
#include <raymath.h>

#include "game/world/units.h"
#include "game/world/world.h"

namespace {
using Control::TrainControls;
using mp_units::quantity;
using mp_units::si::hour;
using mp_units::si::kilo;
using mp_units::si::metre;
using mp_units::si::second;

constexpr auto kMaxTrainVelocity = quantity(80.0 * kilo<metre> / hour).in(metre / second);
constexpr auto kMinTrainVelocity = quantity(0.0 * metre / second);
constexpr auto kTrainMaxAcceleration = 1.5 * (metre / (second * second));
constexpr auto kTrainMaxDeceleration = 5.0 * (metre / (second * second));
constexpr auto kTrainMinimumReverseSpeed = quantity(0.1 * kilo<metre> / hour).in(metre / second);

constexpr auto kCarBogieDistance = -0.4 * metre;
constexpr auto kCarDistance = -4.6 * metre;

constexpr float kModelScale{0.3f};
constexpr Vector3 kModelScale3D{kModelScale, kModelScale, kModelScale};
constexpr Vector3 kModelRotationAxis{0.0, 0.0, 1.0};

float fToNumericalFromOrigin(World::Coordinate coordinate) {
  return static_cast<float>(coordinate.quantity_from(World::origin).numerical_value_in(metre));
}

Vector3 ToRaylibVector3(const World::WorldSpaceCoordinates& world_space_coordinates) {
  float x = fToNumericalFromOrigin(world_space_coordinates.x);
  float y = fToNumericalFromOrigin(world_space_coordinates.y);
  float z = fToNumericalFromOrigin(world_space_coordinates.z);
  return {.x = x, .y = y, .z = z};
}
}  // namespace

Train::Train(const Rails& rails, const Rails::Location location, const int car_count)
    : rails_(rails), cars_(car_count) {
  int current_car{};
  std::generate(std::begin(cars_), std::end(cars_), [this, &current_car, &location]() {
    auto car_front = rails_.Traverse(location, kCarDistance * current_car++);
    auto car_back = rails_.Traverse(car_front, kCarBogieDistance);
    return std::make_pair(car_front, car_back);
  });

  model_ = LoadModel("resources/lowpoly_berlin_u-bahn/untitled.glb");
}

Train::~Train() { UnloadModel(model_); }

void Train::Control(const TrainControls& controls, const Units::TimeDelta time) {
  if (controls.reverse && speed_ < kTrainMinimumReverseSpeed) {
    direction_ = direction_ < 0 ? 1.0f : -1.0f;
  }

  auto acceleration = controls.throttle * kTrainMaxAcceleration;
  auto deceleration = controls.brake * kTrainMaxDeceleration;

  auto new_speed = speed_ + (acceleration * time) - (deceleration * time);
  speed_ = std::clamp(new_speed, kMinTrainVelocity, kMaxTrainVelocity);

  auto distance = speed_ * time * direction_;

  for (auto& car : cars_) {
    auto new_location = rails_.Traverse(car.first, distance);
    if (new_location == car.first) {
      speed_ = 0.0 * metre / second;
    }
    car.first = new_location;
    car.second = rails_.Traverse(car.first, kCarBogieDistance);
  }
}

void Train::Draw() const {
  for (auto& car : cars_) {
    const auto position_1 = rails_.WorldSpace(car.first);
    const auto position_2 = rails_.WorldSpace(car.second);

    Vector3 point_1 = ToRaylibVector3(position_1);
    Vector3 point_2 = ToRaylibVector3(position_2);
    Vector3 point_tmp = Vector3Subtract(point_2, point_1);
    float angle = (atan2(point_tmp.y, point_tmp.x) * (180.0f / PI)) - 90.0f;
    DrawModelEx(model_, point_1, kModelRotationAxis, angle, kModelScale3D, YELLOW);
  }
}

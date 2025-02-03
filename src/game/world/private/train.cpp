#include "game/world/train.h"

#include <absl/log/log.h>
#include <raylib.h>
#include <raymath.h>

#include <utility>

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

constexpr Vector3 kDebugDrawPointSize{0.2f, 0.2f, 2.0f};
constexpr Color kDebugDrawFrontColor{BLACK};

float fToNumericalFromOrigin(World::Coordinate coordinate) {
  return static_cast<float>(coordinate.quantity_from(World::origin).numerical_value_in(metre));
}

/**
 * Tries to apply the requested distance to each train car, if a car does not move then we stop the following cars.
 * We use a separate function for this to simply the view usage,
 * as `cars_` and `cars_ | reverse` are incompatible types.
 */
bool ApplyDistance(const Rails& rails, const Units::Distance distance, auto&& cars) {
  for (auto& car : cars) {
    auto [new_first, success_first] = rails.Traverse(car.first, distance);
    auto [new_second, success_second] = rails.Traverse(new_first, kCarBogieDistance);
    if (!(success_first && success_second)) {
      return false;
    }
    car.first = new_first;
    car.second = new_second;
  }
  return true;
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
    auto [car_front, result_front] = rails_.Traverse(location, kCarDistance * current_car++);
    auto [car_back, result_back] = rails_.Traverse(car_front, kCarBogieDistance);
    if (!(result_front && result_back)) {
      // Note the increment of current_car happening a few lines earlier, making this log effectively 1 indexed
      LOG(WARNING) << "Incorrect initial placement for car #" << current_car;
    }
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

  bool distance_applied{false};
  // Iterate in direction of travel, so that collisions are detected properly
  if (std::signbit(direction_)) {
    distance_applied = ApplyDistance(rails_, distance, cars_ | std::views::reverse);
  } else {
    distance_applied = ApplyDistance(rails_, distance, cars_);
  }
  if (!distance_applied) {
    speed_ = 0.0 * metre / second;
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

void Train::DrawDebug() const {
  // Indicate the front of the train
  const auto front_position = rails_.WorldSpace(cars_.begin()->first);
  const auto back_position = rails_.WorldSpace(cars_.begin()->second);
  DrawCubeV(ToRaylibVector3(front_position), kDebugDrawPointSize, kDebugDrawFrontColor);
  DrawCubeV(ToRaylibVector3(back_position), kDebugDrawPointSize, kDebugDrawFrontColor);

  // Show train debug text
}

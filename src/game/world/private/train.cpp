#include "game/world/train.h"

#include <absl/log/log.h>

#include <ranges>
#include <utility>

#include "game/world/units.h"
#include "game/world/world.h"
#include "third_party/raylib/raylib.h"
#include "third_party/raylib/raymath.h"

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

constexpr auto kCarBogieDistance = -9.5 * metre;
constexpr auto kCarDistance = -16.05 * metre;

constexpr float kModelScale{1.0f};
constexpr Raylib::Vector3 kModelScale3D{kModelScale, kModelScale, kModelScale};
constexpr Raylib::Vector3 kModelRotationAxis{0.0, 0.0, 1.0};

constexpr Raylib::Vector3 kDebugDrawPointSize{0.2f, 0.2f, 2.0f};
constexpr auto kDebugDrawFrontColor{Raylib::BLACK};

float fToNumericalFromOrigin(const World::Coordinate coordinate) {
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

Raylib::Vector3 ToRaylibVector3(const World::WorldSpaceCoordinates& world_space_coordinates) {
  float x = fToNumericalFromOrigin(world_space_coordinates.x);
  float y = fToNumericalFromOrigin(world_space_coordinates.y);
  float z = fToNumericalFromOrigin(world_space_coordinates.z);
  return {.x = x, .y = y, .z = z};
}
}  // namespace

Train::Train(const Rails& rails, const Rails::Location& location, const int car_count)
    : rails_(rails), cars_(car_count) {
  int current_car{};
  std::ranges::generate(cars_, [this, &current_car, &location]() {
    auto [car_front, result_front] = rails_.Traverse(location, kCarDistance * current_car++);
    auto [car_back, result_back] = rails_.Traverse(car_front, kCarBogieDistance);
    if (!(result_front && result_back)) {
      // Note the increment of current_car happening a few lines earlier, making this log effectively 1 indexed
      LOG(WARNING) << "Incorrect initial placement for car #" << current_car;
    }
    return std::make_pair(car_front, car_back);
  });

  model_ = Raylib::LoadModel("resources/berlin/f7x-attempt-1.glb");
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

World::WorldSpaceCoordinates Train::GetCenterPoint() const {
  const auto first_bogie = cars_.front().first;
  const auto last_bogie = cars_.back().second;
  return rails_.WorldSpace(first_bogie).average(rails_.WorldSpace(last_bogie));
}

void Train::Draw() const {
  unsigned car_index = 0;
  for (const auto& [fst, snd] : cars_) {
    const auto position_1 = rails_.WorldSpace(fst);
    const auto position_2 = rails_.WorldSpace(snd);
    const auto average_position = position_1.average(position_2);
    const Raylib::Vector3 point_1 = ToRaylibVector3(position_1);
    const Raylib::Vector3 car_origin = ToRaylibVector3(average_position);
    const Raylib::Vector3 delta = Vector3Subtract(car_origin, point_1);

    // Every second car is reversed, this results in pairs of back to back cars
    const float car_orientation_angle = (car_index % 2) ? 180.0f : 0.0f;
    const float angle = (atan2f(delta.y, delta.x) * (180.0f / PI)) + car_orientation_angle;
    Raylib::DrawModelEx(model_, car_origin, kModelRotationAxis, angle, kModelScale3D, Raylib::WHITE);
    ++car_index;
  }
}

void Train::DrawDebug() const {
  // Indicate the front of the train
  const auto front_position = rails_.WorldSpace(cars_.begin()->first);
  const auto back_position = rails_.WorldSpace(cars_.begin()->second);
  DrawCubeV(ToRaylibVector3(front_position), kDebugDrawPointSize, kDebugDrawFrontColor);
  DrawCubeV(ToRaylibVector3(back_position), kDebugDrawPointSize, kDebugDrawFrontColor);
}

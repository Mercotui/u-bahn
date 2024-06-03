#include "game/world/train.h"

#include <raylib.h>
#include <raymath.h>

#include "game/world/units.h"

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

constexpr float kModelScale{0.3f};
}  // namespace

Train::Train(const Rails& rails, Rails::Location location)
    : rails_(rails), location_(std::make_pair(location, location)) {
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
  // TODO(Menno 23.05.2024) Let rails use Units::Distance in metre.
  auto new_location = rails_.Traverse(location_.first, static_cast<float>(distance.numerical_value_in(metre)));
  if (new_location == location_.first) {
    speed_ = 0.0 * metre / second;
  }
  location_.first = new_location;
  location_.second = rails_.Traverse(location_.first, -0.5f);
}

void Train::Draw() const {
  const auto position_1 = rails_.WorldSpace(location_.first);
  const auto position_2 = rails_.WorldSpace(location_.second);

  Vector2 point_1{.x = position_1.x, .y = position_1.y};
  Vector2 point_2{.x = position_2.x, .y = position_2.y};
  Vector2 point_tmp = Vector2Subtract(point_2, point_1);
  float angle = (atan2(point_tmp.y, point_tmp.x) * (180.0f / PI)) - 90.0f;
  DrawModelEx(model_, {position_1.x, position_1.y, position_1.z}, {0.0, 0.0, 1.0}, angle,
              {kModelScale, kModelScale, kModelScale}, YELLOW);
}

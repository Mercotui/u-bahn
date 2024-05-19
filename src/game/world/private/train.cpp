#include "game/world/train.h"

namespace {
using Control::TrainControls;
constexpr float kMaxTrainVelocity{1.0f};
constexpr float kMinTrainVelocity{-kMaxTrainVelocity};
constexpr float kModelScale{0.3f};
}  // namespace

Train::Train(const Rails& rails, Rails::Location location) : rails_(rails), location_(location) {
  model_ = LoadModel("/home/menno/Workspace/u-bahn/resources/lowpoly_berlin_u-bahn/untitled.glb");
}

Train::~Train() { UnloadModel(model_); }

void Train::Control(const TrainControls& controls) {
  velocity_ += 0.0005f * controls.throttle;
  velocity_ = std::clamp(velocity_, kMinTrainVelocity, kMaxTrainVelocity);

  // TODO(Menno 12.05.2024) distance should equal velocity_ multiplied by time delta
  float distance = velocity_;
  auto new_location = rails_.Traverse(location_, distance);
  if (new_location == location_) {
    velocity_ = 0.0f;
  }
  location_ = new_location;
}

void Train::Draw() {
  const auto position = rails_.WorldSpace(location_);
  DrawModelEx(model_, {position.x, position.y, position.z}, {0.0f, 0.0, 1.0f}, 90.0f,
              {kModelScale, kModelScale, kModelScale}, YELLOW);
}

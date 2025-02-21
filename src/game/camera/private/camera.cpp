// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/camera/private/camera.h"

#include <mp-units/math.h>
#include <mp-units/quantity.h>

#include <memory>

#include "third_party/raylib/raymath.h"

namespace {
using mp_units::si::metre;
using mp_units::si::radian;
using mp_units::si::second;
using std::numbers::pi;

constexpr auto kCameraAngularSpeed = 0.2 * radian / second;

float fToNumericalFromOrigin(const World::Coordinate coordinate) {
  return static_cast<float>(coordinate.quantity_from(World::origin).numerical_value_in(metre));
}

float fGetOpposite(const Units::Angle angle, const Units::Distance hypotenuse) {
  return std::sinf(static_cast<float>(angle.numerical_value_in(radian))) * hypotenuse.numerical_value_in(metre);
}

float fGetAdjacent(const Units::Angle angle, const Units::Distance hypotenuse) {
  return std::cosf(static_cast<float>(angle.numerical_value_in(radian))) * hypotenuse.numerical_value_in(metre);
}

Raylib::Vector3 ToRaylibVector3(const World::WorldSpaceCoordinates& world_space_coordinates) {
  const float x = fToNumericalFromOrigin(world_space_coordinates.x);
  const float y = fToNumericalFromOrigin(world_space_coordinates.y);
  const float z = fToNumericalFromOrigin(world_space_coordinates.z);
  return {.x = x, .y = y, .z = z};
}
}  // namespace

namespace CameraFactory {
std::unique_ptr<CameraInterface> Create() { return std::make_unique<Camera>(); }
}  // namespace CameraFactory

Camera::Camera() : tracking_yaw_(0.7 * radian), tracking_pitch_(0.7 * radian), tracking_distance_(40 * metre) {
  camera_.up = {0.0f, 0.0f, 1.0f};
  camera_.fovy = 45.0f;
  camera_.projection = Raylib::CAMERA_PERSPECTIVE;
}

void Camera::Activate() {
  const auto z = fGetOpposite(tracking_pitch_, tracking_distance_);
  const auto pitch_xy_component = fGetAdjacent(tracking_pitch_, tracking_distance_) * metre;
  const auto x = fGetOpposite(tracking_yaw_, pitch_xy_component);
  const auto y = fGetAdjacent(tracking_yaw_, pitch_xy_component);
  camera_.position = Raylib::Vector3Add(camera_.target, {x, y, z});
  Raylib::BeginMode3D(camera_);
}

void Camera::Deactivate() { Raylib::EndMode3D(); }

void Camera::Control(const Control::CameraControls& controls, Units::TimeDelta time) {
  tracking_yaw_ += controls.x * kCameraAngularSpeed * time;

  const auto new_pitch = tracking_pitch_ + controls.y * kCameraAngularSpeed * time;
  tracking_pitch_ = std::clamp(new_pitch, 0.05 * pi * radian, 0.45 * pi * radian);
}

void Camera::Target(const World::WorldSpaceCoordinates& point) { camera_.target = ToRaylibVector3(point); }

// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT

#include "game/rendering/private/camera.h"

#include <mp-units/framework.h>
#include <mp-units/math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace {
using mp_units::si::metre;
using mp_units::si::radian;
using mp_units::si::second;
using std::numbers::pi;

constexpr auto kCameraAngularSpeed = 0.2 * radian / second;
constexpr auto kFieldOfView = 0.8f;

float fToNumericalFromOrigin(const World::Coordinate coordinate) {
  return static_cast<float>(coordinate.quantity_from(World::origin).numerical_value_in(metre));
}

float fGetOpposite(const Units::Angle angle, const Units::Distance hypotenuse) {
  const auto f_angle = static_cast<float>(angle.numerical_value_in(radian));
  const auto f_hypotenuse = static_cast<float>(hypotenuse.numerical_value_in(metre));
  return std::sinf(f_angle) * f_hypotenuse;
}

float fGetAdjacent(const Units::Angle angle, const Units::Distance hypotenuse) {
  const auto f_angle = static_cast<float>(angle.numerical_value_in(radian));
  const auto f_hypotenuse = static_cast<float>(hypotenuse.numerical_value_in(metre));
  return std::cosf(f_angle) * f_hypotenuse;
}

glm::vec3 vec3Translation(const World::WorldSpaceCoordinates& world_space_coordinates) {
  const float x = fToNumericalFromOrigin(world_space_coordinates.x);
  const float y = fToNumericalFromOrigin(world_space_coordinates.y);
  const float z = fToNumericalFromOrigin(world_space_coordinates.z);
  return {x, y, z};
}

glm::vec3 vec3Rotation(const Units::Angle pitch, const Units::Angle yaw) {
  auto f_pitch = static_cast<float>(pitch.numerical_value_in(radian));
  auto f_yaw = static_cast<float>(yaw.numerical_value_in(radian));
  return {f_pitch, f_yaw, 0.0f};
}
}  // namespace

namespace CameraFactory {
std::unique_ptr<CameraInterface> Create() { return std::make_unique<Camera>(); }
}  // namespace CameraFactory

Camera::Camera() : tracking_yaw_(0.7 * radian), tracking_pitch_(0.7 * radian), tracking_distance_(10 * metre) {
  // TODO(Menno 02.12.2025) The aspect ratio should be updated on window resize events
  perspective_ = glm::perspective(kFieldOfView, 1.0f, 1.0f, 1000.0f);
}

glm::mat4 Camera::Transform() const {
  auto position = vec3Translation(target_);
  const auto pitch_xy_component = fGetAdjacent(tracking_pitch_, tracking_distance_) * metre;
  position.x = fGetOpposite(tracking_yaw_, pitch_xy_component);
  position.y = fGetAdjacent(tracking_yaw_, pitch_xy_component);
  position.z += fGetOpposite(tracking_pitch_, tracking_distance_);

  const auto rotation = vec3Rotation(tracking_pitch_, tracking_yaw_);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), -position);
  view = glm::rotate(view, -rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::rotate(view, -rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
  return perspective_ * view;
}

void Camera::Control(const Control::CameraControls& controls, Units::TimeDelta time) {
  tracking_yaw_ += controls.x * kCameraAngularSpeed * time;

  const auto new_pitch = tracking_pitch_ + controls.y * kCameraAngularSpeed * time;
  tracking_pitch_ = std::clamp(new_pitch, 0.05 * pi * radian, 0.45 * pi * radian);
}

void Camera::Target(const World::WorldSpaceCoordinates& point) { target_ = point; }

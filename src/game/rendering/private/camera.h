// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "game/rendering/camera_interface.h"
#include "game/world/units.h"

class Camera final : public CameraInterface {
 public:
  Camera();

  /**
   * Use camera parameters to calculate the view transform
   * @return the camera's transformation matrix
   */
  glm::mat4 Transform() const override;

  /**
   * Control the camera
   * @param controls the current controls
   * @param time delta time since last frame
   */
  void Control(const Control::CameraControls& controls, Units::TimeDelta time) override;

  /**
   * Target the camera at a point in 3D space
   * @param point the point to target
   */
  void Target(const World::WorldSpaceCoordinates& point) override;

 private:
  World::WorldSpaceCoordinates target_;
  Units::Angle tracking_yaw_;
  Units::Angle tracking_pitch_;
  Units::Distance tracking_distance_;
  glm::mat4 perspective_;
};

// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <game/control/control.h>
#include <game/world/world.h>

#include <memory>

class CameraInterface {
 public:
  virtual ~CameraInterface() = default;

  /**
   * Activate the camera
   * Locks in the camera parameters, changing the angles will not affect the camera until `Activate` is called again.
   */
  virtual void Activate() = 0;

  /**
   * Deactivate the camera
   */
  virtual void Deactivate() = 0;

  /**
   * Control the camera
   * @param controls the current controls
   * @param time delta time since last frame
   */
  virtual void Control(const Control::CameraControls& controls, Units::TimeDelta time) = 0;

  /**
   * Target the camera at a point in 3D space
   * @param point the point to target
   */
  virtual void Target(const World::WorldSpaceCoordinates& point) = 0;
};

namespace CameraFactory {
std::unique_ptr<CameraInterface> Create();
}

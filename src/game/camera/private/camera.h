#pragma once

#include "game/camera/camera_interface.h"
#include "game/world/units.h"
#include "third_party/raylib/raylib.h"

class Camera final : public CameraInterface {
 public:
  Camera();

  /**
   * Activate this camera.
   * Locks in the camera parameters, changing the angles will not affect the camera until `Activate` is called again.
   */
  void Activate() override;

  /**
   * End the camera
   */
  void Deactivate() override;

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
  Units::Angle tracking_yaw_;
  Units::Angle tracking_pitch_;
  Units::Distance tracking_distance_;
  Raylib::Camera camera_{};
};

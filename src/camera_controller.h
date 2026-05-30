#pragma once
#include "fpv_camera.h"


class CameraController {
 public:
  CameraController(FPVCamera& camera);

  void Update(float delta_time, glm::vec2 mouse_motion);
 private:
  FPVCamera& camera_;
  float move_speed_;
  float rotation_speed_;
  float pitch_;
  float yaw_;
};
#pragma once

#include "direction.h"
#include "frustrum.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct CameraShaderData {
  glm::mat4 view_matrix;
  glm::mat4 projection_matrix;
  glm::vec3 position;
};

struct FPVCamera {
  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::quat rotation = {1.0f, 0.0f, 0.0f, 0.0f};

  float fov = 60.0f;
  float aspect_ratio = 16.0f / 9.0f;
  float near_plane = 0.1f;
  float far_plane = 100.0f;
  
  glm::mat4 GetProjectionMat() const;
  glm::mat4 GetViewMat() const;
  Frustrum GetFrustrum() const;
  void Move(Direction direction, float distance);
};
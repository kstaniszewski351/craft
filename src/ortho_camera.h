#pragma once

#include <glm/gtc/quaternion.hpp>


struct OrthoCamera {
  glm::vec3 position = {};
  glm::quat rotation = glm::identity<glm::quat>();

  float aspect_ratio = 1.0f;
  float size = 1.0f;

  glm::mat4 GetProjectionMat() const;
  glm::mat4 GetViewMat() const;
};
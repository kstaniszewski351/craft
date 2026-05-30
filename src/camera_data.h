#pragma once
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

struct CameraData {
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 position;
};
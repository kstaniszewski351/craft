#pragma once

#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

struct Transform {
  glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
  glm::quat rotation = glm::identity<glm::quat>();

  glm::mat4 GetMat();

};
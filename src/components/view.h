#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

struct View {
  float zOffset = 0.0f;
  glm::quat rotation = glm::identity<glm::quat>();
};
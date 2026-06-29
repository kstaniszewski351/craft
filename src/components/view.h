#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ray.h"

struct View {
  float yOffset = 0.0f;
  float length = 4.0f;
  glm::quat rotation = glm::identity<glm::quat>();

  Ray GetRay(glm::vec3 pos) const;
};
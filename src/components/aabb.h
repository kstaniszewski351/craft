#pragma once

#include <glm/vec3.hpp>

struct AABB {
  AABB(glm::vec3 size) : size(size) {}
  glm::vec3 size;
};
#pragma once

#include <glm/vec3.hpp>

struct BoxCollider {
  BoxCollider(glm::vec3 size) : size(size) {}
  glm::vec3 size;
};
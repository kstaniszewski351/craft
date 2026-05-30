#pragma once

#include <glm/vec3.hpp>

struct PhysicsObject {
  glm::vec3 velocity = glm::vec3();
  bool onGround = false;
};
#pragma once

#include <glm/ext/vector_float3.hpp>

struct Ray {
  glm::vec3 dir;
  glm::vec3 pos;
  float length;
};
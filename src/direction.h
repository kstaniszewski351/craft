#include <glm/vec3.hpp>
#include <array>
#pragma once

enum Direction {
  Up = 0,
  Down,
  Left,
  Right,
  Front,
  Back,
  DirectionCount
};

constexpr std::array<glm::ivec3, DirectionCount> DIRECTION_VECTORS_I {
  glm::ivec3(0, 1, 0),
  glm::ivec3(0, -1, 0),
  glm::ivec3(-1, 0, 0),
  glm::ivec3(1, 0, 0),
  glm::ivec3(0, 0, -1),
  glm::ivec3(0, 0, 1)
};

constexpr std::array<glm::vec3, DirectionCount> DIRECTION_VECTORS {
  glm::vec3(0.0f, 1.0f, 0.0f),
  glm::vec3(0.0f, -1.0f, 0.0f),
  glm::vec3(-1.0f, 0.0f, 0.0f),
  glm::vec3(1.0f, 0.0f, 0.0f),
  glm::vec3(0.0f, 0.0f, -1.0f),
  glm::vec3(0.0f, 0.0f, 1.0f),
};
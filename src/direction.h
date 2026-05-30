#include <glm/vec3.hpp>
#include <array>
#pragma once

enum Direction {
  Up,
  Down,
  Left,
  Right,
  Front,
  Back,
  DirectionCount
};

constexpr std::array<glm::vec3, DirectionCount> direction_vectors {
  glm::vec3(0.0f, 1.0f, 0.0f),
  glm::vec3(0.0f, -1.0f, 0.0f),
  glm::vec3(-1.0f, 0.0f, 0.0f),
  glm::vec3(1.0f, 0.0f, 0.0f),
  glm::vec3(0.0f, 0.0f, -1.0f),
  glm::vec3(0.0f, 0.0f, 1.0f),
};
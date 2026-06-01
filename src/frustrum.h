#pragma once

#include <glm/vec3.hpp>

struct Plane {
  Plane() = default;
  Plane(glm::vec3 normal, glm::vec3 pos);
  glm::vec3 normal = {0.0f, 1.0f, 0.0f};
  float distance = 0.0f;
  float GetSignedDistance(glm::vec3 point) const;
};

struct Frustrum {
  Plane right_plane;
  Plane left_plane;
  Plane top_plane;
  Plane bottom_plane;
  Plane near_plane;
  Plane far_plane;
};

struct AABB {
  glm::vec3 center;
  glm::vec3 extents;
  bool IsOnForwardPlane(const Plane& plane) const;
  bool IsInFrustrum(const Frustrum& frustrum) const;
};


#include "frustrum.h"
#include <cstdlib>
#include <glm/geometric.hpp>

float Plane::GetSignedDistance(glm::vec3 point) const {
  return glm::dot(normal, point) - distance;
}

bool AABB::IsOnForwardPlane(const Plane& plane) const {
      const float r = extents.x * std::abs(plane.normal.x) +
            extents.y * std::abs(plane.normal.y) + extents.z * std::abs(plane.normal.z);

    return -r <= plane.GetSignedDistance(center);
}

bool AABB::IsInFrustrum(const Frustrum& frustrum) const {
  return IsOnForwardPlane(frustrum.near_plane) &&
  IsOnForwardPlane(frustrum.far_plane) &&
  IsOnForwardPlane(frustrum.right_plane) &&
  IsOnForwardPlane(frustrum.left_plane) &&
  IsOnForwardPlane(frustrum.top_plane) &&
  IsOnForwardPlane(frustrum.bottom_plane);
}
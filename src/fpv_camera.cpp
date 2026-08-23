#include "fpv_camera.h"
#include "direction.h"
#include "frustrum.h"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>

glm::mat4 FPVCamera::GetViewMat() const {
  glm::mat4 view_matrix = glm::mat4(1.0f);
  glm::mat4 rotation_matrix = glm::mat4_cast(glm::inverse(rotation));

  view_matrix = view_matrix * rotation_matrix;
  view_matrix = glm::translate(view_matrix, -position);

  

  return view_matrix;
}

glm::mat4 FPVCamera::GetProjectionMat() const {
  glm::mat4 projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);

  return projection_matrix;
}

void FPVCamera::Move(Direction direction, float distance) {

}

Plane::Plane(glm::vec3 normal, glm::vec3 pos) {
  this->normal = glm::normalize(normal);
  distance = glm::dot(this->normal, pos);
}

Frustrum FPVCamera::GetFrustrum() const {
  Frustrum f;
  glm::vec3 up = rotation * DIRECTION_VECTORS[Up];
  glm::vec3 front = rotation * DIRECTION_VECTORS[Front];
  glm::vec3 right = rotation * DIRECTION_VECTORS[Right];
  glm::vec3 far_vec = far_plane * front;
  float half_h = far_plane * tanf(fov * 0.5f);
  float half_w = half_h * aspect_ratio;

  f.near_plane = Plane(front, position + near_plane * front);
  f.far_plane = Plane(-front, position + far_vec);
  f.right_plane = Plane(glm::cross(far_vec - right * half_w, up), position);
  f.left_plane = Plane(glm::cross(up, far_vec + right * half_w), position);
  f.top_plane = Plane(glm::cross(right, far_vec - up * half_h), position);
  f.bottom_plane = Plane(glm::cross(far_vec + up * half_h, right), position);
  return f;
}
#include "fpv_camera.h"

glm::mat4 FPVCamera::GetViewMat() {
  glm::mat4 view_matrix = glm::mat4(1.0f);
  glm::mat4 rotation_matrix = glm::mat4_cast(glm::inverse(rotation));

  view_matrix = view_matrix * rotation_matrix;
  view_matrix = glm::translate(view_matrix, -position);

  

  return view_matrix;
}

glm::mat4 FPVCamera::GetProjectionMat() {
  glm::mat4 projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);

  return projection_matrix;
}

void FPVCamera::Move(Direction direction, float distance) {

}
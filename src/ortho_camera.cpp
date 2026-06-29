#include "ortho_camera.h"
#include <glm/ext/matrix_clip_space.hpp>

glm::mat4 OrthoCamera::GetProjectionMat() const {
  return glm::ortho(-(size/2.0f)*aspect_ratio, (size/2.0f)*aspect_ratio, -(size/2.0f), (size/2.0f), 0.1f, 100.0f);
}

glm::mat4 OrthoCamera::GetViewMat() const {
  glm::mat4 view_matrix = glm::mat4(1.0f);
  glm::mat4 rotation_matrix = glm::mat4_cast(glm::inverse(rotation));

  view_matrix = view_matrix * rotation_matrix;
  view_matrix = glm::translate(view_matrix, -position);

  

  return view_matrix;
}
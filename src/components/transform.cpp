#include "transform.h"

glm::mat4 Transform::GetMat() {
  glm::mat4 mat = glm::mat4_cast(rotation);
  mat = glm::translate(mat, pos);

  return mat;
}
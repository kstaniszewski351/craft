#include "transform.h"
#include <glm/ext/matrix_float4x4.hpp>

glm::mat4 Transform::GetMat() {
  return  glm::translate(glm::identity<glm::mat4>(), pos);
}
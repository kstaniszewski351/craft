#include "view.h"
#include "direction.h"

Ray View::GetRay(glm::vec3 pos) const {
  return Ray(rotation * direction_vectors[Front], {pos.x, pos.y + yOffset, pos.z}, length);
}
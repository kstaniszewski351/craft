#include "view.h"
#include "direction.h"

Ray View::GetRay(glm::vec3 pos) const {
  return Ray(rotation * DIRECTION_VECTORS[Front], {pos.x, pos.y + yOffset, pos.z}, length);
}
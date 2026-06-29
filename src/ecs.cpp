#include "ecs.h"
#include "entity.h"
#include <entt/entity/fwd.hpp>

Entity ECS::AddEntity() {
  return Entity(this, reg_.create());
}

entt::registry& ECS::GetReg() {
  return reg_;
}
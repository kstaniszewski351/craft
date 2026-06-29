#include "entity.h"
#include "ecs.h"
#include <entt/entity/fwd.hpp>

Entity::Entity(ECS* ecs, entt::entity ent)
 : ecs_(ecs), ent_(ent) {

}
#include "entity.h"
#include <entt/entity/fwd.hpp>

Entity::Entity(entt::registry* reg, entt::entity ent)
 : reg_(reg), ent_(ent) {

}
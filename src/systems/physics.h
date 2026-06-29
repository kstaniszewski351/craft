#pragma once

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include "world.h"
#include "components/box_collider.h"
#include "components//transform.h"

void Physics(entt::registry& reg, World& world, float delta_time);
bool Collides(const Transform& transform, const BoxCollider& aabb, glm::ivec3 block_pos);
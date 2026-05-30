#pragma once

#include <entt/entt.hpp>
#include "../window.h"
#include "../world.h"


void MovePlayer(entt::registry& reg, Window& window,  World& world, float delta_time);
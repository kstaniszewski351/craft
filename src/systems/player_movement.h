#pragma once

#include <entt/entt.hpp>
#include "window.h"
#include "scene.h"


void MovePlayer(ECS& reg, Window& window,  Scene& world, float delta_time);
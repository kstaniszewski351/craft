#pragma once

#include "entity.h"
#include "systems/hitbox_renderer.h"
#include "window.h"
#include "world.h"
#include "world_renderer.h"
#include <entt/entt.hpp>
#include "fpv_camera.h"

class Scene {
  public:
    Scene();
    void Draw(float delta_time, Window& window);
    Entity AddEntity();
  private:
    void updateCameraData();
    World world_;
    entt::registry reg_;
    WorldRenderer world_renderer_;
    HitboxRenderer hitbox_renderer_;
    Entity player_;
    FPVCamera camera_;
    Buffer camera_ebo_;
};
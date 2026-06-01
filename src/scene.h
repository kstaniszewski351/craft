#pragma once

#include "camera_data.h"
#include "systems/hitbox_renderer.h"
#include "window.h"
#include "world.h"
#include "world_renderer.h"
#include "components/view.h"
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include "fpv_camera.h"

class Scene {
  public:
    Scene();
    void Draw(float delta_time, Window& window);
  private:
    void updateCameraData();
    World world_;
    WorldRenderer world_renderer_;
    HitboxRenderer hitbox_renderer_;
    entt::entity player_;
    FPVCamera camera_;
    entt::registry reg_;
    Buffer camera_ebo_;
};
#pragma once

#include "gui/gui_renderer.h"
#include "gui/hotbar.h"
#include "systems/hitbox_renderer.h"
#include "window.h"
#include "world.h"
#include "world_renderer.h"
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include "fpv_camera.h"

class Entity;

class Scene {
  public:
    Scene(const Atlas& atlas);
    void Draw(float delta_time, Window& window);
    World& GetWorld();
  private:
    void updateCameraData();
    World world_;
    ECS entities_;
    WorldRenderer world_renderer_;
    GUI::GUIRenderer gui_renderer_;
    HitboxRenderer hitbox_renderer_;
    GUI::GUIRenderer gui_renderer;
    GUI::Hotbar* hotbar_;
    Entity player_;
    FPVCamera camera_;
    GFX::Buffer camera_ebo_;
  friend class Entity;
};
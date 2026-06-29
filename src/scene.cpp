#include "scene.h"
#include "camera_data.h"
#include "components/box_collider.h"
#include "components/inventory.h"
#include "components/physics_object.h"
#include "components/player_controller.h"
#include "components/transform.h"
#include "components/view.h"
#include "ecs.h"
#include "entity.h"
#include "gui/hotbar.h"
#include "item_stack.h"
#include "registry.h"
#include "systems/hitbox_renderer.h"
#include "systems/physics.h"
#include "systems/player_movement.h"
#include "window.h"
#include <entt/entity/fwd.hpp>
#include "registries.h"

Scene::Scene(const Atlas& atlas)
  : world_renderer_(world_, atlas) 
  {
  player_ = entities_.AddEntity();
  player_.AddComponent<Transform>(glm::vec3(1, 7, 10));
  player_.AddComponent<BoxCollider>(glm::vec3(0.8, 1.8, 0.8));
  player_.AddComponent<PhysicsObject>();
  player_.AddComponent<View>(View({.yOffset = 1.6f}));
  player_.AddComponent<PlayerController>();
  auto& inv = player_.AddComponent<Inventory>();
  auto& dirt = gItemRegistry.Get("dirt");
  inv.slots[0] = ItemStack{.item = &dirt, .size = 1};
  inv.active_slot = 5;

  //hotbar_ = new GUI::Hotbar({0, 0}, inv);
  camera_.fov = 90;
}

void Scene::Draw(float delta_time, Window& window) {
  //update
  auto player_transform = player_.GetComponent<Transform>();

  world_.UpdateLoaded(player_transform.pos);
  world_renderer_.Update();

  MovePlayer(entities_, window, *this, delta_time);

  Physics(entities_.GetReg(), world_, delta_time);

  //camera
  updateCameraData();


  //draw
  world_renderer_.Draw(camera_);
  hitbox_renderer_.Draw(entities_.GetReg());

  //gui_renderer.Draw(*hotbar_, window);
}

void Scene::updateCameraData() {
  auto view = player_.GetComponent<View>();
  auto transform = player_.GetComponent<Transform>();

  glm::vec3 pos = transform.pos;
  pos.y += view.yOffset;
  // pos.x += hitbox.size.x / 2;
  // pos.z += hitbox.size.z / 2;

  camera_.position = pos;
  camera_.rotation = view.rotation;
  CameraData camera_data {
    .view = camera_.GetViewMat(),
    .projection = camera_.GetProjectionMat(),
    .position = camera_.position
  };
  camera_ebo_.BindTarget(GL_UNIFORM_BUFFER, 0);
  camera_ebo_.Data(sizeof(camera_data), &camera_data);
}

World& Scene::GetWorld() {
  return world_;
}

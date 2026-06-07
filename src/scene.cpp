#include "scene.h"
#include "camera_data.h"
#include "components/box_collider.h"
#include "components/physics_object.h"
#include "components/player_controller.h"
#include "components/transform.h"
#include "components/view.h"
#include "entity.h"
#include "systems/hitbox_renderer.h"
#include "systems/physics.h"
#include "systems/player_movement.h"
#include "window.h"
#include <entt/entity/fwd.hpp>

Scene::Scene()
  : world_renderer_(world_),
  hitbox_renderer_(reg_),
  player_(Entity(&reg_, reg_.create()))
  {
  player_ = Entity(&reg_, reg_.create());
  player_.AddComponent<Transform>(glm::vec3(1, 7, 10));
  player_.AddComponent<BoxCollider>(glm::vec3(0.8, 1.8, 0.8));
  player_.AddComponent<PhysicsObject>();
  player_.AddComponent<View>(View({.zOffset = 1.6f}));
  player_.AddComponent<PlayerController>();
  camera_.fov = 90;
}

Entity Scene::AddEntity() {
  return Entity(&reg_, reg_.create());
}

void Scene::Draw(float delta_time, Window& window) {
  //update
  auto player_transform = player_.GetComponent<Transform>();

  world_.UpdateLoaded(player_transform.pos);
  world_renderer_.Update();

  MovePlayer(reg_, window, world_, delta_time);

  Physics(reg_, world_, delta_time);

  //camera
  updateCameraData();


  //draw
  world_renderer_.Draw(camera_);
  hitbox_renderer_.Draw();
}

void Scene::updateCameraData() {
  auto view = player_.GetComponent<View>();
  auto transform = player_.GetComponent<Transform>();
  auto hitbox = player_.GetComponent<BoxCollider>();

  glm::vec3 pos = transform.pos;
  pos.y += view.zOffset;
  pos.x += hitbox.size.x / 2;
  pos.z += hitbox.size.z / 2;

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


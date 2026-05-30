#include "scene.h"
#include "camera_data.h"
#include "components/aabb.h"
#include "components/physics_object.h"
#include "components/player_controller.h"
#include "components/transform.h"
#include "components/view.h"
#include "systems/hitbox_renderer.h"
#include "systems/physics.h"
#include "systems/player_movement.h"
#include "window.h"
#include <entt/entity/fwd.hpp>

Scene::Scene()
  : world_renderer_(world_),
  hitbox_renderer_(reg_)
  {

  auto entity = reg_.create();
  player_ = entity;
  reg_.emplace<Transform>(entity, Transform({
    .pos = glm::vec3(1, 7, 10)
  }));
  reg_.emplace<AABB>(entity, glm::vec3(0.8, 1.8, 0.8));
  reg_.emplace<PhysicsObject>(entity);
  reg_.emplace<View>(entity, View({
    .zOffset = 1.6
  }));
  reg_.emplace<PlayerController>(entity);
  camera.fov = 90;
}

void Scene::Draw(float delta_time, Window& window) {
  //update
  auto player_transform = reg_.get<Transform>(player_);

  world_.UpdateLoaded(player_transform.pos);
  world_renderer_.Update();

  MovePlayer(reg_, window, world_, delta_time);

  Physics(reg_, world_, delta_time);

  //camera
  updateCameraData();
  CameraData camera_data {
    .view = camera.GetViewMat(),
    .projection = camera.GetProjectionMat(),
    .position = camera.position
  };
  camera_ebo_.BindTarget(GL_UNIFORM_BUFFER, 0);
  camera_ebo_.Data(sizeof(camera_data), &camera_data);

  //draw
  world_renderer_.Draw();
  hitbox_renderer_.Draw();
}

void Scene::updateCameraData() {
  auto [view, tranform, hitbox] = reg_.get<View, Transform, AABB>(player_);

  glm::vec3 pos = tranform.pos;
  pos.y += view.zOffset;
  pos.x += hitbox.size.x / 2;
  pos.z += hitbox.size.z / 2;

  camera.position = pos;
  camera.rotation = view.rotation;
}


#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include "ecs.h"


class Entity {
 public:
  Entity() = default;
  Entity(ECS* ecs, entt::entity ent);
  template<typename T, typename... Args>
  auto& AddComponent(Args&&... args) {
    return ecs_->reg_.emplace<T>(ent_, std::forward<Args>(args)...);
  };
  template<typename T>
  T GetComponent() {
    return ecs_->reg_.get<T>(ent_);
  }
 private:
  ECS* ecs_ = nullptr;
  entt::entity ent_ = entt::null;
};
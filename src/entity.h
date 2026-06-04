#pragma once
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <utility>

class Entity {
 public:
  Entity(entt::registry* reg, entt::entity ent);
  template<typename T, typename... Args>
  void AddComponent(Args&&... args) {
    reg_->emplace<T>(ent_, std::forward<Args>(args)...);
  };
  template<typename T>
  T GetComponent() {
    return reg_->get<T>(ent_);
  }
 private:
  entt::registry* reg_;
  entt::entity ent_;
};
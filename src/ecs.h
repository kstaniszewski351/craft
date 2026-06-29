#pragma once

#include <entt/entt.hpp>

class Entity;

class ECS {
 public:
  Entity AddEntity();
  entt::registry& GetReg();
 private:
  entt::registry reg_;
 friend class Entity;
};
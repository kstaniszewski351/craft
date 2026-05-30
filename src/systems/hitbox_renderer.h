#pragma once

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include "../shader.h"
#include "../buffer.h"

class HitboxRenderer {
 public:
  HitboxRenderer(entt::registry& reg);
  void Draw();
 private:
  entt::registry& reg_;
  Shader shader_; 
  Buffer vbo_;
  Buffer ebo_;
  Buffer ubo_;
  GLuint vao_;
};
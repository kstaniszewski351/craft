#pragma once

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include "gfx/shader.h"
#include "gfx/buffer.h"
#include "gfx/vao.h"

class HitboxRenderer {
 public:
  HitboxRenderer();
  void Draw(entt::registry& reg);
 private:
  GFX::Shader shader_; 
  GFX::Buffer vbo_;
  GFX::Buffer ebo_;
  GFX::Buffer ubo_;
  GFX::VAO vao_;
};
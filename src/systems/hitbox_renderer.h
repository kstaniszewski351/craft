#pragma once

#include "bgfx/bgfx.h"
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

class HitboxRenderer {
 public:
  HitboxRenderer();
  ~HitboxRenderer();
  void Draw(entt::registry& reg);
 private:
  bgfx::ProgramHandle shader_;
  bgfx::VertexBufferHandle vertex_buf_;
  bgfx::IndexBufferHandle index_buf_;
};
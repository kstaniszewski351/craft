#pragma once

#include "bgfx/bgfx.h"
#include "gui/gui_renderer.h"
#include "widget.h"

namespace GUI {
  class Image : public Widget {
   public:
    Image(bgfx::TextureHandle texture, glm::ivec2 pos, glm::ivec2 size, glm::vec2 uv_offset = {0.0f, 0.0f}, glm::vec2 uv_scale = {1.0f, 1.0f});
    void DrawImpl(GUIRenderer& renderer) override;
   private:
    bgfx::TextureHandle texture_;
    glm::vec2 uv_offset_;
    glm::vec2 uv_scale_;
  };
}
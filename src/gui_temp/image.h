#pragma once

#include "gui/gui_renderer.h"
#include "gui/widget.h"
#include "gfx/texture.h"
#include "rect.h"

namespace GUI {
  class Image : public Widget {
   public:
    Image(const GFX::Texture* texture, Rect uv = {});
    void SetUV(Rect uv);
   private:
    void DrawImpl(GUIRenderer& renderer) override;
    const GFX::Texture* texture_;
    GFX::Buffer uv_;
    glm::ivec2 size_;
  };
}
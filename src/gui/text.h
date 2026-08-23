#pragma once

#include "bgfx/bgfx.h"
#include "font.h"
#include "gui/gui_renderer.h"
#include "widget.h"
namespace GUI {
 class Text : public Widget {
  friend class GUIRenderer;
  public:
   ~Text();
   void Draw(GUIRenderer& renderer) const;
  private:
    Text(std::string text, glm::ivec2 pos, Font* font, const bgfx::VertexLayout& layout);
    glm::ivec2 Recalc();
    Font* font_;
    bgfx::DynamicVertexBufferHandle vertex_buf_;
    bgfx::DynamicIndexBufferHandle index_buf_;
    std::string text_;
 };
}
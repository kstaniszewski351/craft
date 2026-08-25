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
   void Recalc() override;
  private:
    Text(std::string text, glm::ivec2 pos, Font* font, const bgfx::VertexLayout& layout);
    Font* font_;
    bgfx::DynamicVertexBufferHandle vertex_buf_;
    bgfx::DynamicIndexBufferHandle index_buf_;
    std::string text_;
 };
}
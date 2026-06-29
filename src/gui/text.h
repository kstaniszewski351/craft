#pragma once

#include "font.h"
#include "gui/gui_renderer.h"
#include "widget.h"
namespace GUI {
 class Text : public Widget {
  public:
   Text(std::string text, glm::ivec2 pos, Font* font);
   void Draw(GUIRenderer& renderer) const;
  private:
    glm::ivec2 Recalc();
    Font* font_;
    GFX::Buffer vbo_;
    GFX::Buffer ebo_;
    std::string text_;
 };
}
#pragma once

#include "gui/gui_renderer.h"
#include "rect.h"
#include <glm/vec2.hpp>

namespace GUI {
  class Widget {
   public:
    Widget(glm::ivec2 pos, glm::ivec2 size);

    glm::ivec2 GetSize() const;
    glm::ivec2 GetPos() const;
    Rect GetBounds() const;
    void SetPos(glm::ivec2 pos);
    virtual void Draw(GUIRenderer& renderer) {};
    virtual void OnClick() {};
    virtual ~Widget() {};
   private:
    glm::ivec2 pos_;
    glm::ivec2 size_;
  };
}
#pragma once

#include "direction.h"
#include "gui/gui_renderer.h"
#include <memory>
#include <vector>

namespace GUI {
  class Widget {
   public:
    enum class AlignMode {
      Left,
      Right,
      Center
    };

    void SetVisible(bool value);
    bool GetVisible() const;
    int GetWidth() const;
    int GetHeight() const;
    void SetPos(glm::ivec2 pos);
    glm::ivec2 GetPos() const;
    void SetParent(Widget* parent);
    Widget* GetParent() const;
    void Draw(GUIRenderer& renderer);
    void Recalculate();
   protected:
    virtual void DrawImpl(GUIRenderer& renderer) {};
    virtual void RecalculateImpl() {};
    glm::ivec2 CalcPos() const;
    Widget* parent_ = nullptr;
    bool visible_ = true;
    glm::ivec2 pos_ = {0, 0};
    glm::ivec2 size_ = {0, 0};
    bool changed_ = true;
    AlignMode align_v = AlignMode::Left;
    AlignMode align_h = AlignMode::Left;
  };
}

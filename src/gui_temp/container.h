#pragma once

#include "gui/gui_renderer.h"
#include "gui/widget.h"


namespace GUI {
  class Container : public Widget {
   public:
    void AddChild(Widget* child);
    void DrawImpl(GUIRenderer& renderer) override;
   protected:
    std::vector<std::unique_ptr<Widget>> children_;
  };
}

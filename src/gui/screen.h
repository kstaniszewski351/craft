#pragma once

#include "gui/gui_manager.h"
#include "gui/widget.h"
#include <memory>
#include <vector>

class Game;

namespace GUI {
  class Screen {
   public:
    Screen();
    ~Screen();
    void Draw(GUIRenderer& renderer);
    void Click(glm::ivec2 pos);
    virtual void Update() {};
    virtual void Recalc(glm::ivec2 size) {};
   protected:
    Widget* AddWidget(Widget* widget);
    GuiManager* gui_manager_;
   private:
    std::vector<std::unique_ptr<Widget>> widgets_;
  };
}
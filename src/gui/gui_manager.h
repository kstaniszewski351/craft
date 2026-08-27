#pragma once

#include "gui/gui_renderer.h"
#include <set>

namespace GUI {
  class Screen;

  class GuiManager {
   public:
    void AddScreen(Screen* screen);
    void RemoveScreen(Screen* screen);
    void SetActive(Screen* screen);
    void Update();
    void Recalc(glm::ivec2 window_size);
    void Draw();
   private:
    std::set<Screen*> screens_;
    glm::ivec2 window_size_;
    bool recalc_;
    GUIRenderer renderer_;
  };
}

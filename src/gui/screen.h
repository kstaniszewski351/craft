#pragma once

#include "game.h"
#include "gui/widget.h"
#include <memory>
#include <vector>
namespace GUI {
  class Screen {
   public:
    Screen(glm::ivec2 size);
    void Draw();
    void Click(glm::ivec2 pos);
    glm::ivec2 GetSize() const;
   protected:
    Widget* AddWidget(Widget* widget);
    Game& game_;
   private:
    std::vector<std::unique_ptr<Widget>> widgets_;
    glm::ivec2 size_;
  };
}
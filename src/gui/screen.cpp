#include "screen.h"
#include "game.h"
#include "gui/gui_renderer.h"
#include <memory>

namespace GUI {
  Screen::Screen(glm::ivec2 size) : game_(Game::Get()), size_(size) {

  }

  void Screen::Draw() {
    for(auto& widget : widgets_) {
      widget->Draw(game_.GetGUIRenderer());
    }
  }

  Widget* Screen::AddWidget(Widget* widget) {
    widgets_.push_back(std::unique_ptr<Widget>(widget));

    return widget;
  }

  void Screen::Click(glm::ivec2 pos) {
    for(auto& widget : widgets_) {
      Rect bounds = widget->GetBounds();
      
      if(
        bounds.p1.x < pos.x && pos.x < bounds.p2.x &&
        bounds.p1.y < pos.x && pos.x < bounds.p2.y) {
        widget->OnClick();
      }
    }
  }

  glm::ivec2 Screen::GetSize() const {
    return size_;
  }
}
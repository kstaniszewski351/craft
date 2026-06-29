#include "container.h"
#include "gui/gui_renderer.h"

namespace GUI {
  void Container::AddChild(Widget* child) {
    child->SetParent(this);
    children_.push_back(std::move(std::unique_ptr<Widget>(child)));
  }
  
  void Container::DrawImpl(GUIRenderer& renderer) {
    for(auto& child : children_) {
      child->Draw(renderer);
    }
  }
}

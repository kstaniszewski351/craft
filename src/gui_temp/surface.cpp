#include "surface.h"
#include "gui/gui_renderer.h"

namespace GUI {
  Surface::Surface(glm::ivec2 size)   {
    size_ = size;
  }
  void Surface::SetSize(glm::ivec2 size) {
    changed_ = true;
    size_ = size;
  }
  void Surface::RecalculateImpl() {
    for(auto& child : children_) {
      child->Recalculate();
    }
  }
}
#include "vert_layout.h"

namespace GUI {

  VertLayout::VertLayout(int width) {
    size_ = {width, 0};
  }

  void VertLayout::RecalculateImpl() {
    size_.y = 0;
    for(auto& child : children_) {
      child->SetPos({0, size_.y});
      child->Recalculate();
      int child_y = child->GetHeight();
      size_.y += child_y;
    }
  }
}
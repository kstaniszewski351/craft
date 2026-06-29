#include "widget.h"

namespace GUI {
  Widget::Widget(glm::ivec2 pos, glm::ivec2 size) : pos_(pos), size_(size) {

  }

  glm::ivec2 Widget::GetPos() const {
    return pos_;
  }

  glm::ivec2 Widget::GetSize() const {
    return size_;
  }

  void Widget::SetPos(glm::ivec2 pos) {
    pos_ = pos;
  }

  Rect Widget::GetBounds() const {
    return {
      pos_,
      pos_ + size_
    };
  }
}
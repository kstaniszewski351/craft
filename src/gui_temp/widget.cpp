#include "widget.h"
#include "gui/gui_renderer.h"
#include <memory>

namespace GUI {

  void Widget::SetVisible(bool value) {
    visible_ = value;
  }

  bool Widget::GetVisible() const {
    return visible_;
  }

  // glm::ivec2 Widget::GetSize() const {
  //   return size_;
  // }

  // void Widget::SetSize(glm::ivec2 size) {
  //   size_ = size;
  //   changed_  = true;
  // }

  void Widget::SetPos(glm::ivec2 pos) {
    pos_ = pos;
    changed_  = true;
  }

  glm::ivec2 Widget::GetPos() const {
    return pos_;
  }



  void Widget::Recalculate() {
    RecalculateImpl();
  }

  void Widget::Draw(GUIRenderer& renderer) {
    // if(changed_) {
    //   Recalculate();
    //   changed_ = false;
    // }
    if(visible_){
      DrawImpl(renderer);
    }
  }
  int Widget::GetHeight() const {
    return size_.y;
  }

  int Widget::GetWidth() const {
    return size_.x;
  }

  Widget* Widget::GetParent() const {
    return parent_;
  }
  void Widget::SetParent(Widget* parent) {
    parent_ = parent;
  }
  glm::ivec2 Widget::CalcPos() const {
    return this->pos_ + parent_->pos_;
  }
}
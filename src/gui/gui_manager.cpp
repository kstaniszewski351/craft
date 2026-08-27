#include "gui_manager.h"
#include "gui/screen.h"

namespace GUI {
  void GuiManager::AddScreen(Screen* screen) {
    screens_.insert(screen);
  }
  void GuiManager::RemoveScreen(Screen* screen) {
    screens_.erase(screen);
  }
  void GuiManager::Draw() {
    if(recalc_) {
      constexpr int height = 300;
      int width = (window_size_.x * height) / window_size_.y;

      for(auto screen : screens_) {
        screen->Recalc({width, height});
      }
      renderer_.Update({width, height});
      recalc_ = false;
    }
    for(auto screen : screens_) {
      screen->Draw(renderer_);
    }
  }
  void GuiManager::Update() {
    for(auto screen : screens_) {
      screen->Update();
    }
  }
  void GuiManager::Recalc(glm::ivec2 window_size) {

    window_size_ = window_size;
    recalc_ = true;
  }
}
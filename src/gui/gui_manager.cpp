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
    constexpr int height = 300;
    int width = (window_size.x * height) / window_size.y;

    for(auto screen : screens_) {
      screen->Recalc({width, height});
    }
    renderer_.Update({width, height});
  }
}
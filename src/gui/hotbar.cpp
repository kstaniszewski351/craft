#include "hotbar.h"
#include "components/inventory.h"
#include "gui/gui_renderer.h"
#include "gui/screen.h"
#include "texture_manager.h"

constexpr glm::ivec2 HOTBAR_SIZE = {188, 20};
constexpr glm::ivec2 SELECTOR_SIZE = {22, 22};

namespace GUI {
  Hotbar::Hotbar(glm::ivec2 pos, const Inventory& inv, glm::ivec2 size) : Screen(size){
    auto& tex_manager = game_.GetTextureManager();

    glm::ivec2 hotbar_pos = {GetSize().x / 2 - HOTBAR_SIZE.x / 2, GetSize().y - HOTBAR_SIZE.y - 20};
    hotbar_img_ = new Image(
      tex_manager.GetTexture("res/textures/hotbar.png"),
      hotbar_pos,
      HOTBAR_SIZE
    );

    AddWidget(hotbar_img_);

    Update(inv);
  }

  void Hotbar::Update(const Inventory& inv) {
    int slot = inv.active_slot;
    selector_img_->SetPos(glm::ivec2(SELECTOR_SIZE.x, 0) * slot);
  }
}
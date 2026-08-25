#include "hotbar.h"
#include "components/inventory.h"
#include "gui/item_slot.h"
#include "gui/screen.h"
#include "game.h"

constexpr glm::ivec2 HOTBAR_SIZE = {188, 20};
constexpr glm::ivec2 SELECTOR_SIZE = {22, 22};

namespace GUI {
  Hotbar::Hotbar(Inventory* inv) : inv_(inv), Screen(){
    auto& tex_manager = Game::Get().GetTextureManager();

    hotbar_img_ = new Image(
      tex_manager.GetTexture("res/textures/hotbar.png"),
      {0, 0},
      HOTBAR_SIZE
    );
    AddWidget(hotbar_img_);

    selector_img_ = new Image(
      tex_manager.GetTexture("res/textures/hotbar_selector.png"),
      {0, 0},
      SELECTOR_SIZE
    );
    AddWidget(selector_img_);

    for(int i = 0; i < 9; i++) {
      ItemSlot* slot= new ItemSlot(&inv_->slots[i], {0, 0});
      AddWidget(slot);
      slots_[i] = slot;
    }
  }

  void Hotbar::updateSelector() {
    selector_img_->SetPos(hotbar_img_->GetPos() + glm::ivec2(21, 0) * inv_->active_slot + glm::ivec2(-1, -1));
  }

  void Hotbar::updateSlots() {
    for(int i = 0; i < 9; i++) {
      ItemSlot* slot = slots_[i];

      slot->SetPos(hotbar_img_->GetPos() + glm::ivec2(21, 0) * i + glm::ivec2(2, 2));
    }
  }

  void Hotbar::Update() {
    auto key_downs = Game::Get().GetInputManager().GetKeyboardDowns();

    for(int i = SDL_SCANCODE_1; i <= SDL_SCANCODE_9; i++) {
      if(key_downs[i]) {
        inv_->active_slot = i - SDL_SCANCODE_1;
        break;
      }
    }
    updateSelector();
  }

  void Hotbar::Recalc(glm::ivec2 size) {
    glm::ivec2 hotbar_pos = {size.x / 2 - HOTBAR_SIZE.x / 2, size.y - HOTBAR_SIZE.y - 10};
    hotbar_img_->SetPos(hotbar_pos);
    updateSelector();
    updateSlots();
  }
}
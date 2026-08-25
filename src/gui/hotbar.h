#pragma once

#include "components/inventory.h"
#include "gui/image.h"
#include "gui/item_slot.h"
#include "gui/screen.h"
#include "gui_renderer.h"


namespace GUI {
  class Hotbar : public Screen {
   public:
    Hotbar(Inventory* inv);
    void Recalc(glm::ivec2 size) override;
    void Update() override;
   private:
    void updateSelector();
    void updateSlots();
    std::array<ItemSlot*, 9> slots_;
    Image* hotbar_img_;
    Image* selector_img_;
    Inventory* inv_;
  };
}
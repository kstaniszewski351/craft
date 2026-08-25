#pragma once

#include "gui/widget.h"
#include "item_stack.h"
#include "image.h"
#include <optional>
namespace GUI {
  class ItemSlot : public Widget {
   public:
    ItemSlot(std::optional<ItemStack>* content, glm::ivec2 pos);
    void DrawImpl(GUIRenderer& renderer) override;
    void OnClick() override;
    void Recalc() override;
   private:
    std::optional<Image> image_;
  };
}
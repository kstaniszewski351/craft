#include "item_slot.h"
#include "gui/gui_renderer.h"
#include "gui/widget.h"
#include "item.h"
#include "registries.h"

namespace GUI {
  constexpr int SLOT_SIZE = 16;

  ItemSlot::ItemSlot(std::optional<ItemStack>* content, glm::ivec2 pos) :
    Widget(pos, {SLOT_SIZE, SLOT_SIZE}) {

    if(!content->has_value()) {
      return;
    }

    int id = content->value().item->GetRegistryID();

    const Item& item = gItemRegistry.Get(id);
    ItemTexInfo tex_info = item.GetTexInfo();
    image_ =  Image(tex_info.texture, {0, 0}, {SLOT_SIZE, SLOT_SIZE}, tex_info.uv_offset, tex_info.uv_scale);
  }

  void ItemSlot::DrawImpl(GUIRenderer& renderer) {
    if(image_) {
      image_->Draw(renderer);
    }
  }

  void ItemSlot::Recalc() {
    if(image_) {
      image_->SetPos(GetPos());
    }
  }

  void ItemSlot::OnClick() {

  }
}
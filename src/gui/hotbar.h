#pragma once

#include "components/inventory.h"
#include "gui/image.h"
#include "gui/screen.h"
#include "gui_renderer.h"

#include "widget.h"

namespace GUI {
  class Hotbar : public Screen {
   public:
    Hotbar(glm::ivec2 pos, const Inventory& inv, glm::ivec2 size);
    void Update(const Inventory& inv);
   private:
    
    Image* hotbar_img_;
    Image* selector_img_;
    
  };
}
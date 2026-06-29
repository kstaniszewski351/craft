#pragma once

#include "gui/container.h"
#include "gui/gui_renderer.h"
#include "gui/widget.h"
namespace GUI {
  class Surface : public Container {
   public:
    Surface(glm::ivec2 size);
    void SetSize(glm::ivec2 size);
    void RecalculateImpl() override;
  };
}
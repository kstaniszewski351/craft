#pragma once

#include "gui/container.h"

namespace GUI {
  class VertLayout : public Container {
   public:
    VertLayout(int width);
    void SetWidth() const;
   protected:
    void RecalculateImpl() override;
   private:
  };
}
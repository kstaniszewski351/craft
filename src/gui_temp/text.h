#pragma once

#include "font.h"
#include "gfx/buffer.h"
#include "widget.h"

namespace GUI {

  class Text : public GUI::Widget {
   public:
    enum class WrapMode {
      None,
      Word,
      Char
    };
    Text(std::string text, Font* font);
    void SetText(std::string text);
    std::string GetText() const;
    void DrawImpl(GUIRenderer& renderer) override;
    void RecalculateImpl() override;
   private:
    Font* font_;
    std::string text_;
    GFX::Buffer vbo_;
    GFX::Buffer ebo_;
    WrapMode wrap_mode_ = WrapMode::Word;
  };
}
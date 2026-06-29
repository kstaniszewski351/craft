#include "image.h"
#include "gui/gui_renderer.h"
#include <array>

namespace GUI {

  Image::Image(const GFX::Texture* texture, Rect uv) :
    texture_(texture) {
    SetUV(uv);
    size_ = {texture->GetWidth(), texture->GetHeight()};
  };

  void Image::SetUV(Rect uv) {
    auto uv_data = std::array<glm::vec2, 4> {{
      uv.p1,
      {uv.p1.x, uv.p2.y},
      {uv.p2.x, uv.p1.y},
      uv.p2
    }};

    uv_.Data(sizeof(uv_data), &uv_data);
  }

  void Image::DrawImpl(GUIRenderer& renderer) {
    renderer.DrawImage(CalcPos(), size_, *texture_, uv_);
  }


}
#include "image.h"

namespace GUI {

  Image::Image(const GFX::Texture* texture, glm::ivec2 pos, glm::ivec2 size, Rect uv) :
   Widget(pos, size),
   texture_(texture) {
    SetUV(uv);

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

  void Image::Draw(GUIRenderer& renderer) {
    renderer.DrawImage(GetPos(), GetSize(), *texture_, uv_);
  }


}
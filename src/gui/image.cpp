#include "image.h"
#include "bgfx/bgfx.h"
#include <glm/ext/vector_float2.hpp>

namespace GUI {

  Image::Image(bgfx::TextureHandle texture, glm::ivec2 pos, glm::ivec2 size, glm::vec2 uv_offset, glm::vec2 uv_scale) :
   Widget(pos, size),
   texture_(texture),
   uv_offset_(uv_offset),
   uv_scale_(uv_scale) {

  };

  void Image::Draw(GUIRenderer& renderer) {
    renderer.DrawImage(GetPos(), GetSize(), texture_, uv_offset_, uv_scale_);
  }


}
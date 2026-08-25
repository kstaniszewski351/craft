#include "text.h"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "gui/gui_renderer.h"

namespace GUI {
  Text::Text(std::string text, glm::ivec2 pos, Font* font, const bgfx::VertexLayout& layout) :
    text_(text),
    font_(font),
    Widget(pos, {0, 0}) {
    vertex_buf_ = bgfx::createDynamicVertexBuffer((unsigned int)0, layout, BGFX_BUFFER_ALLOW_RESIZE);
    index_buf_ = bgfx::createDynamicIndexBuffer((unsigned int)0, BGFX_BUFFER_ALLOW_RESIZE);
  }
  Text::~Text() {
    bgfx::destroy(vertex_buf_);
    bgfx::destroy(index_buf_);
  }

  void Text::Recalc() {
    std::vector<QuadVertex> vertices;
    vertices.reserve(text_.size() * QUAD_VERTS.size());
    std::vector<uint16_t> ebo;
    ebo.reserve(text_.size() * QUAD_TRIANGLES.size());

    //int max_w = size_.x;

    const Atlas& atlas = font_->GetAtlas();
    glm::ivec2 calc_pos = GetPos();
    glm::ivec2 offset = {0, 0};

    int i =0;
    for(char c : text_) {
      const Font::Character& character = font_->GetCharacter(c);

      // if(offset.x + character.advance > max_w) {
      //   offset.x = 0;
      //   offset.y += font_->GetHeight();
      // }

      glm::vec2 char_pos = calc_pos +offset + glm::ivec2(character.bearing.x, -character.bearing.y + font_->GetHeight());
      glm::vec2 char_uv = atlas.GetUV(character.atlas_id);
      glm::vec2 base_uv = (glm::vec2)character.size / (glm::vec2)atlas.GetSize();
      for(glm::vec2 vert_pos : QUAD_VERTS) {
        glm::vec2 pos = char_pos + vert_pos * (glm::vec2)character.size;
        glm::vec2 uv = char_uv + base_uv * vert_pos;
        vertices.push_back(QuadVertex({pos, uv}));
      }      
      for(unsigned int index : QUAD_TRIANGLES) {
        ebo.push_back(index + i * QUAD_VERTS.size());
      }

      offset.x += character.advance;
      i++;
    }
    bgfx::update(vertex_buf_, 0, bgfx::copy(vertices.data(), vertices.size() * sizeof(QuadVertex)));
    bgfx::update(index_buf_, 0, bgfx::copy(ebo.data(), ebo.size() * sizeof(uint16_t)));
    //return offset;
  }
}
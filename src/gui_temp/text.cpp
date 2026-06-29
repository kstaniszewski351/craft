#include "text.h"
#include "gui/gui_renderer.h"
#include <glm/ext/vector_float2.hpp>

namespace GUI {
  Text::Text(std::string text, Font* font) :
    text_(text),
    font_(font) {
  }

  void Text::RecalculateImpl() {
    std::vector<QuadVertex> vertices;
    vertices.reserve(text_.size() * QUAD_VERTS.size());
    std::vector<unsigned int> ebo;
    ebo.reserve(text_.size() * QUAD_TRIANGLES.size());

    int max_w = parent_->GetWidth();

    const Atlas& atlas = font_->GetAtlas();
    glm::ivec2 calc_pos = CalcPos();
    glm::ivec2 offset = {0, 0};

    int i =0;
    for(char c : text_) {
      const Font::Character& character = font_->GetCharacter(c);

      if(offset.x + character.advance > max_w) {
        offset.x = 0;
        offset.y += font_->GetHeight();
      }

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
    size_ = offset;
    ebo_.Data(sizeof(unsigned int) * ebo.size(), ebo.data());
    vbo_.Data(sizeof(QuadVertex) * vertices.size(), vertices.data());
  };

  void Text::DrawImpl(GUIRenderer& renderer) {
    renderer.DrawText(font_->GetAtlas().getTexture(), vbo_, ebo_, text_.size());
  }

  void Text::SetText(std::string text) {
    changed_ = true;
    text_ = text;
  }
  
  std::string Text::GetText() const {
    return text_;
  }
}
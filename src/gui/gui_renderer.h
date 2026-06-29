#pragma once

#include "gfx/buffer.h"
#include "gfx/shader.h"
#include "gfx/texture.h"
#include "gfx/vao.h"
#include "window.h"
#include <array>


namespace GUI {
  class Widget;

  struct QuadVertex {
    glm::vec2 pos;
    glm::vec2 uv;
  };
  
  constexpr std::array<glm::vec2, 4> QUAD_VERTS = {{
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f}
  }};

  constexpr std::array<unsigned int, 6> QUAD_TRIANGLES = {
    0,
    1,
    2,
    2,
    1,
    3
  };

  class GUIRenderer {
   public:
    GUIRenderer();
    void DrawImage(glm::ivec2 pos, glm::ivec2 size, const GFX::Texture& texture, const GFX::Buffer& uv);
    void DrawText(const GFX::Texture& texture, const GFX::Buffer& vbo, const GFX::Buffer& ebo, int count);
    void DrawQuad();
    void Draw(Widget& root, const Window& window);
   private:
    GFX::Shader rect_shader_;
    GFX::Shader text_shader_;
    GFX::Buffer rect_vbo_;
    GFX::Buffer rect_ebo_;
    GFX::VAO text_vao_;
    GFX::VAO rect_vao_;
  };
}
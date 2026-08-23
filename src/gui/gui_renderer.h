#pragma once

#include "bgfx/bgfx.h"
#include "font.h"
#include "window.h"
#include <array>
#include <cstdint>
#include <memory>


namespace GUI {
  class Widget;
  class Text;

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

  constexpr std::array<uint16_t, 6> QUAD_TRIANGLES = {
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
    void DrawImage(glm::ivec2 pos, glm::ivec2 size, bgfx::TextureHandle texture, glm::vec2 uv_offset, glm::vec2 uv_scale);
    void DrawText(bgfx::TextureHandle texture, bgfx::VertexBufferHandle vertex_buf, bgfx::IndexBufferHandle index_buf, int count);
    Text* CreateText(std::string text, glm::ivec2 pos, Font* font);
    void DrawQuad();
    void Draw(Widget& root, const Window& window);
    ~GUIRenderer();
   private:
    bgfx::VertexLayout text_layout_;
    bgfx::ProgramHandle rect_shader_;
    bgfx::ProgramHandle text_shader_;
    bgfx::VertexBufferHandle rect_vertex_buf_;
    bgfx::IndexBufferHandle rect_index_buf_;
    bgfx::UniformHandle s_text_atlas_;
    bgfx::UniformHandle s_image_;
    bgfx::UniformHandle u_transform;
    bgfx::UniformHandle u_uv_;
  };
}
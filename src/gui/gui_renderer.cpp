#include "gui_renderer.h"
#include "gfx/buffer.h"
#include "gfx/texture.h"
#include "gfx/vao.h"
#include <array>
#include <cstddef>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "widget.h"




namespace GUI {



  constexpr std::array<GFX::VAO::Attrib, 2> QUAD_VERTEX_FORMAT {{
    {2, GL_FLOAT, 0, false, 0},
    {2, GL_FLOAT, 0, false, 1}
  }};

  constexpr std::array<GFX::VAO::Attrib, 2> TEXT_VERTEX_FORMAT {{
    {2, GL_FLOAT, 0},
    {2, GL_FLOAT, offsetof(QuadVertex, uv)}
  }};

  GUIRenderer::GUIRenderer() :
    rect_vao_(QUAD_VERTEX_FORMAT.begin(), QUAD_VERTEX_FORMAT.end(), 0),
    rect_shader_("res/shaders/rect.frag", "res/shaders/rect.vert"),
    text_shader_("res/shaders/text.frag", "res/shaders/text.vert"),
    text_vao_(TEXT_VERTEX_FORMAT.begin(), TEXT_VERTEX_FORMAT.end(), sizeof(QuadVertex)) {
    
    rect_ebo_.Data(sizeof(QUAD_TRIANGLES), QUAD_TRIANGLES.data());
    rect_vbo_.Data(sizeof(QUAD_VERTS), QUAD_VERTS.data());
  };

  void GUIRenderer::Draw(Widget& root, const Window& window) {
    glm::vec2 window_size = window.GetSize();
    glm::mat4 proj_mat = glm::ortho(0.0f, (float)window_size.x, (float)window_size.y, 0.0f);


    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    rect_shader_.SetUniform(0, proj_mat);
    text_shader_.SetUniform(0, proj_mat);
    root.Draw(*this);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
  }

  void GUIRenderer::DrawImage(glm::ivec2 pos, glm::ivec2 size, const GFX::Texture& texture, const GFX::Buffer& uv) {
    rect_shader_.Use();
    rect_vao_.Bind();
    rect_vbo_.BindVertexBuffer(0, 0, sizeof(glm::vec2));
    uv.BindVertexBuffer(1, 0, sizeof(glm::vec2));
    rect_ebo_.Bind(GL_ELEMENT_ARRAY_BUFFER);
    texture.Bind(0);
    glm::vec2 s = size;
    glm::vec2 p = pos;
    rect_shader_.SetUniform(1, p);
    rect_shader_.SetUniform(2, s);

    glDrawElements(GL_TRIANGLES, QUAD_TRIANGLES.size(), GL_UNSIGNED_INT, 0);
  }

  void GUIRenderer::DrawText(const GFX::Texture& texture, const GFX::Buffer& vbo, const GFX::Buffer& ebo, int count) {
    text_shader_.Use();
    text_vao_.Bind();
    ebo.Bind(GL_ELEMENT_ARRAY_BUFFER);
    vbo.BindVertexBuffer(0, 0, sizeof(QuadVertex));
    texture.Bind(0);

    glDrawElements(GL_TRIANGLES, count * QUAD_TRIANGLES.size(), GL_UNSIGNED_INT, 0);
  }
}
#include "gui_renderer.h"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "game.h"
#include <array>
#include <cstddef>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include "gui/text.h"
#include "shader_manager.h"
#include "widget.h"




namespace GUI {

  //constexpr auto GUI_STATE = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_NEVER | BGFX_STATE_MSAA | BGFX_STATE_CULL_CW;
  constexpr auto GUI_STATE =  
  BGFX_STATE_WRITE_RGB |
  BGFX_STATE_WRITE_A |
  BGFX_STATE_CULL_CW |
  BGFX_STATE_MSAA |
  BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
  GUIRenderer::GUIRenderer() {
    
    bgfx::VertexLayout quad_layout;
    quad_layout.begin()
      .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
    .end();

    text_layout_ = bgfx::VertexLayout();
    text_layout_.begin()
      .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
      .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
    .end();

    rect_vertex_buf_ = bgfx::createVertexBuffer(
      bgfx::makeRef(QUAD_VERTS.data(),
      QUAD_VERTS.size() * sizeof(glm::vec2)),
      quad_layout
    );
    rect_index_buf_ = bgfx::createIndexBuffer(
      bgfx::makeRef(QUAD_TRIANGLES.data(),
      QUAD_TRIANGLES.size() * sizeof(uint16_t))
    );

    ShaderManager& shader_manager = Game::Get().GetShaderManager();

    rect_shader_ = shader_manager.LoadProgram("rect_vs.sc", "rect_fs.sc");
    text_shader_ = shader_manager.LoadProgram("text_vs.sc", "text_fs.sc");

    s_text_atlas_ = bgfx::createUniform("s_textAtlas", bgfx::UniformType::Sampler);
    s_image_ = bgfx::createUniform("s_image", bgfx::UniformType::Sampler);
    u_transform = bgfx::createUniform("u_transform", bgfx::UniformType::Vec4);
    u_uv_ = bgfx::createUniform("u_uv", bgfx::UniformType::Vec4);
    u_gui_proj_ = bgfx::createUniform("u_guiProj", bgfx::UniformType::Mat4);
  };

  Text* GUIRenderer::CreateText(std::string text, glm::ivec2 pos, Font* font) {
    return new Text(text, pos, font, text_layout_);
  }

  void GUIRenderer::Update(glm::ivec2 window_size) {
    glm::mat4 proj_mat = glm::ortho(0.0f, (float)window_size.x, (float)window_size.y, 0.0f, -1.0f, 1.0f);
    bgfx::setUniform(u_gui_proj_, &proj_mat);
  }

  void GUIRenderer::DrawImage(glm::ivec2 pos, glm::ivec2 size, bgfx::TextureHandle texture, glm::vec2 uv_offset, glm::vec2 uv_scale) {
    bgfx::setState(GUI_STATE);
    bgfx::setIndexBuffer(rect_index_buf_);
    bgfx::setVertexBuffer(0, rect_vertex_buf_);
    bgfx::setTexture(0, s_image_, texture);

    glm::vec4 transform = {pos, size};
    glm::vec4 uv = {uv_offset, uv_scale};
    bgfx::setUniform(u_transform, &transform);
    bgfx::setUniform(u_uv_, &uv);

    bgfx::submit(0, rect_shader_);
  }

  void GUIRenderer::DrawText(bgfx::TextureHandle texture, bgfx::VertexBufferHandle vertex_buf, bgfx::IndexBufferHandle index_buf, int count) {
    bgfx::setState(GUI_STATE);
    bgfx::setVertexBuffer(0, vertex_buf);
    bgfx::setIndexBuffer(index_buf);
    bgfx::setTexture(0, s_text_atlas_, texture);

    bgfx::submit(0, text_shader_);
  }

  GUIRenderer::~GUIRenderer() {
    bgfx::destroy(rect_shader_);
    bgfx::destroy(text_shader_);
    bgfx::destroy(rect_vertex_buf_);
    bgfx::destroy(rect_index_buf_);
    bgfx::destroy(s_text_atlas_);
    bgfx::destroy(s_image_);
    bgfx::destroy(u_uv_);
    bgfx::destroy(u_transform);
    bgfx::destroy(u_gui_proj_);
  }
}
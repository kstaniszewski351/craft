#include "hitbox_renderer.h"
#include <cstdint>
#include <entt/entity/fwd.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "components/transform.h"
#include "components/box_collider.h"
#include "game.h"

constexpr std::array<glm::vec3, 8> CUBE_VERTS {
    glm::vec3(-0.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(0.0f, 1.0f, 1.0f)
};

constexpr std::array<uint16_t, 12*2> CUBE_LINES {
    0, 1,
    1, 2,
    2, 3,
    3, 0,
    4, 5,
    5, 6,
    6, 7,
    7, 4,
    0, 4,
    1, 5,
    2, 6,
    3, 7
};


HitboxRenderer::HitboxRenderer() {
  shader_ = Game::Get().GetShaderManager().LoadProgram("debug_vs.sc", "debug_fs.sc");

  bgfx::VertexLayout layout;
  layout.begin()
    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
  .end();

  vertex_buf_ = bgfx::createVertexBuffer(
    bgfx::makeRef(CUBE_VERTS.data(), CUBE_VERTS.size() * sizeof(glm::vec3)),
    layout
  );

  index_buf_ = bgfx::createIndexBuffer(bgfx::makeRef(CUBE_LINES.data(), CUBE_LINES.size() * sizeof(uint16_t)));
};

void HitboxRenderer::Draw(entt::registry& reg) {
  auto view = reg.view<Transform, BoxCollider>();
  bgfx::setVertexBuffer(0, vertex_buf_);
  bgfx::setIndexBuffer(index_buf_);
  bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_PT_LINES);

  for(auto [entity, transform, box] : view.each()) {
    glm::vec3 pos = transform.pos;
    pos -= glm::vec3(box.size.x / 2.0f, 0.0f, box.size.z / 2.0f);

    glm::mat4 model_mat = glm::translate(glm::identity<glm::mat4>(), pos);
    model_mat = glm::scale(model_mat, box.size);
    
    bgfx::setTransform(&model_mat);
    bgfx::submit(0, shader_);
  }
}

HitboxRenderer::~HitboxRenderer() {
  bgfx::destroy(shader_);
  bgfx::destroy(vertex_buf_);
  bgfx::destroy(index_buf_);
}
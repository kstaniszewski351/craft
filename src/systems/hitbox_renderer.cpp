#include "hitbox_renderer.h"
#include <entt/entity/fwd.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include "components/transform.h"
#include "components/box_collider.h"

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

constexpr std::array<unsigned int, 12*2> CUBE_LINES {
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

constexpr std::array<GFX::VAO::Attrib, 1> HITBOX_VERTEX_FORMAT {
  {3, GL_FLOAT, 0}
};

HitboxRenderer::HitboxRenderer()
  : shader_("res/shaders/debug.frag", "res/shaders/debug.vert"),
    vao_(HITBOX_VERTEX_FORMAT.begin(), HITBOX_VERTEX_FORMAT.end(), sizeof(glm::vec3)) {
  vbo_.Data(sizeof(CUBE_VERTS), &CUBE_VERTS);
  ebo_.Data(sizeof(CUBE_LINES), &CUBE_LINES);
};

void HitboxRenderer::Draw(entt::registry& reg) {
  auto view = reg.view<Transform, BoxCollider>();
  shader_.Use();
  vao_.Bind();
  ebo_.Bind(GL_ELEMENT_ARRAY_BUFFER);
  vbo_.BindVertexBuffer(0, 0, sizeof(glm::vec3));
  ubo_.BindTarget(GL_UNIFORM_BUFFER, 1);

  for(auto [entity, transform, box] : view.each()) {
    glm::vec3 pos = transform.pos;
    pos -= glm::vec3(box.size.x / 2.0f, 0.0f, box.size.z / 2.0f);

    glm::mat4 model_mat = glm::translate(glm::identity<glm::mat4>(), pos);
    model_mat = glm::scale(model_mat, box.size);
    
    ubo_.Data(sizeof(glm::mat4), &model_mat);
    glDrawElements(GL_LINES, CUBE_LINES.size(), GL_UNSIGNED_INT, 0);
  }
}
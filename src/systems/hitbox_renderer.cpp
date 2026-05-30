#include "hitbox_renderer.h"
#include <entt/entity/fwd.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../components/transform.h"
#include "../components/aabb.h"

constexpr std::array<glm::vec3, 8> CUBE_VERTS {
    glm::vec3(0.0f, 0.0f, 0.0f),  // 0: (0,0,0)
    glm::vec3(1.0f, 0.0f, 0.0f),  // 1: (1,0,0)
    glm::vec3(1.0f, 1.0f, 0.0f),  // 2: (1,1,0)
    glm::vec3(0.0f, 1.0f, 0.0f),  // 3: (0,1,0)
    glm::vec3(0.0f, 0.0f, 1.0f),  // 4: (0,0,1)
    glm::vec3(1.0f, 0.0f, 1.0f),  // 5: (1,0,1)
    glm::vec3(1.0f, 1.0f, 1.0f),  // 6: (1,1,1)
    glm::vec3(0.0f, 1.0f, 1.0f)   // 7: (0,1,1)
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

// struct ModelData {
//   glm::vec3 pos;
//   glm::vec3 scale;
// };

HitboxRenderer::HitboxRenderer(entt::registry& reg) : reg_(reg), shader_("res/shaders/debug.frag", "res/shaders/debug.vert") {
  glCreateVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  vbo_.Bind(GL_ARRAY_BUFFER);
  ebo_.Bind(GL_ELEMENT_ARRAY_BUFFER);
  vbo_.Data(sizeof(CUBE_VERTS), &CUBE_VERTS);
  ebo_.Data(sizeof(CUBE_LINES), &CUBE_LINES);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
};

void HitboxRenderer::Draw() {
  auto view = reg_.view<Transform, AABB>();
  shader_.Use();
  ubo_.BindTarget(GL_UNIFORM_BUFFER, 1);
  glBindVertexArray(vao_);

  for(auto [entity, transform, box] : view.each()) {
    
    glm::mat4 model_mat = glm::translate(glm::identity<glm::mat4>(), transform.pos);
    model_mat = glm::scale(model_mat, box.size);
    
    ubo_.Data(sizeof(glm::mat4), &model_mat);
    glDrawElements(GL_LINES, CUBE_LINES.size(), GL_UNSIGNED_INT, 0);
  }
}
#pragma once

#include "atlas.h"
#include "fpv_camera.h"
#include "world.h"
#include "chunk_mesh.h"
#include <bgfx/bgfx.h>
#include <glm/vec2.hpp>
#include <memory>
#include <unordered_map>

// constexpr std::array<GFX::VAO::Attrib, 2> CHUNK_VERTEX_FORMAT {{
//   {3, GL_FLOAT, offsetof(ChunkVertex, pos)},
//   {2, GL_FLOAT, offsetof(ChunkVertex, uv)}
// }};


class WorldRenderer {
 public:
  WorldRenderer(World&, const Atlas& atlas);
  ~WorldRenderer();
  void Update();
  void Draw(const FPVCamera& camera);
 private:
  World& world_;
  const Atlas& atlas_;
  std::unordered_map<glm::ivec2, std::unique_ptr<ChunkMesh>> meshes_;
  bgfx::VertexLayout vertex_layout_;
  bgfx::ProgramHandle chunk_shader_;
  bgfx::UniformHandle atlas_handle_;
};
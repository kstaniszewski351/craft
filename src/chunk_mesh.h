#pragma once

#include "atlas.h"
#include "chunk.h"
#include <bgfx/bgfx.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include "direction.h"
#include "chunk_vertex.h"

void addBlockFace(Direction dir, glm::ivec3 pos, char block_id, char light_level, const Atlas& atlas, std::vector<ChunkVertex>& vertices, std::vector<std::uint16_t>& triangles);

class ChunkMesh {
 public:
  ChunkMesh(const Chunk& chunk, const World& world, const Atlas& atlas, const bgfx::VertexLayout& layout);
  ~ChunkMesh();
  void Bind();
 private:
  bgfx::VertexBufferHandle vertex_buffer_;
  bgfx::IndexBufferHandle index_buffer_;
};
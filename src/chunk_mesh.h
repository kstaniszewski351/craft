#pragma once

#include "atlas.h"
#include "chunk.h"
#include <bgfx/bgfx.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include "direction.h"
#include "chunk_vertex.h"

void addBlockFace(Direction dir, glm::ivec3 pos, char block_id, const Atlas& atlas, std::vector<ChunkVertex>& vertices, std::vector<std::uint16_t>& triangles);

class ChunkMesh {
 public:
  ChunkMesh(Chunk* chunk, const Atlas* atlas, const bgfx::VertexLayout& layout);
  ~ChunkMesh();
  void Update();
  void Bind();
 private:
  Chunk* chunk_;
  void addFace(Direction dir, glm::ivec3 pos, char block, std::vector<ChunkVertex>& vertices, std::vector<unsigned int>& triangles);
  bgfx::DynamicVertexBufferHandle vertex_buffer_;
  bgfx::DynamicIndexBufferHandle index_buffer_;
  const Atlas* atlas_;
};
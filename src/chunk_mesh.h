#pragma once

#include "atlas.h"
#include "gfx/buffer.h"
#include "chunk.h"
#include <glm/mat4x4.hpp>
#include <vector>
#include "glad/gl.h"
#include "direction.h"
#include "chunk_vertex.h"

void addBlockFace(Direction dir, glm::ivec3 pos, char block_id, const Atlas& atlas, std::vector<ChunkVertex>& vertices, std::vector<unsigned int>& triangles);

class ChunkMesh {
 public:
  ChunkMesh(Chunk& chunk, const Atlas& atlas);
  ~ChunkMesh();
  void Update();
  void Draw();
  std::vector<ChunkVertex> vertices;
  std::vector<GLuint> triangles;
 private:
  Chunk& chunk_;
  void addFace(Direction dir, glm::ivec3 pos, char block);
  GFX::Buffer vbo_;
  GFX::Buffer ebo_;
  const Atlas& atlas_;
};
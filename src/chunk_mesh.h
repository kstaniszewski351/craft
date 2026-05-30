#pragma once

#include "atlas.h"
#include "buffer.h"
#include "chunk.h"
#include <glm/mat4x4.hpp>
#include <vector>
#include "glad/glad.h"
#include "direction.h"

struct ChunkVertex {
  glm::vec3 pos;
  glm::vec2 uv;
};

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
  Buffer vbo_;
  Buffer ebo_;
  GLuint vao_;
  const Atlas& atlas_;
};
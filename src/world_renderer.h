#pragma once

#include "atlas.h"
#include "shader.h"
#include "world.h"
#include "chunk_mesh.h"
#include <glm/vec2.hpp>
#include <memory>
#include <unordered_map>

class WorldRenderer {
 public:
  WorldRenderer(World&);
  const Atlas& GetAtlas(); 

  void Update();
  void Draw();
 private:
  World& world_;
  //GLuint model_data_buffer_;
  //GLuint vao_;
  Shader chunk_shader_;
  Atlas atlas_;
  std::unordered_map<glm::ivec2, std::unique_ptr<ChunkMesh>> meshes_;
};
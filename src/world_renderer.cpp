#include "world_renderer.h"
#include "atlas.h"
#include "bitmap.h"
#include "block_registry.h"
#include "chunk_mesh.h"
#include "shader.h"
#include "texture.h"
#include "world.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include <utility>

WorldRenderer::WorldRenderer(World& world) : 
  world_(world),
  chunk_shader_("res/shaders/chunk.frag", "res/shaders/chunk.vert"),
  atlas_(gBlockRegistry.BuildAtlas())
  {
  //glGenBuffers(1, &model_data_buffer_);
}



void WorldRenderer::Update() {
  //remove unloaded chunks
  auto& loaded_chunks = world_.GetChunks();

  for(auto it = meshes_.cbegin(); it != meshes_.cend();) {
    if(!loaded_chunks.contains(it->first)) {
      meshes_.erase(it++);
    }
    else {
      it++;
    }
  }

  
  
  //add newly loaded

  for(auto& chunk : loaded_chunks) {
    if(!meshes_.contains(chunk.first)) {
      meshes_[chunk.first] = std::make_unique<ChunkMesh>(chunk.second, atlas_);
    }
  }

  //update meshes
  for(auto& mesh : meshes_) {
    mesh.second->Update();
  }
}

void WorldRenderer::Draw() {
  chunk_shader_.Use();
  atlas_.getTexture().Bind(0);
  //glBindBufferBase(GL_UNIFORM_BUFFER, 1, model_data_buffer_);
  //glBindVertexArray(vao_);

  for(const auto& mesh : meshes_) {
    
    //glm::ivec2 chunk_pos = mesh.first;
  
    //glBindBuffer(GL_UNIFORM_BUFFER, model_data_buffer_);
    //glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::ivec2), &chunk_pos, GL_STATIC_DRAW);

    mesh.second->Draw();
    
  }
}

const Atlas& WorldRenderer::GetAtlas() {
  return atlas_;
}
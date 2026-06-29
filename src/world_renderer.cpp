#include "world_renderer.h"
#include "atlas.h"
#include "chunk_mesh.h"
#include "fpv_camera.h"
#include "frustrum.h"
#include "gfx/shader.h"
#include "gfx/texture.h"
#include "world.h"
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <utility>
#include <vector>




WorldRenderer::WorldRenderer(World& world, const Atlas& atlas) : 
  world_(world),
  chunk_shader_("res/shaders/chunk.frag", "res/shaders/chunk.vert"),
  atlas_(atlas),
  vao_(CHUNK_VERTEX_FORMAT.begin(), CHUNK_VERTEX_FORMAT.end(), sizeof(ChunkVertex))
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

void WorldRenderer::Draw(const FPVCamera& camera) {

  Frustrum frustrum = camera.GetFrustrum();
  //crate draw list

  std::vector<std::pair<float, ChunkMesh*>> draw_list;

  for(auto& [pos, mesh] : meshes_) {
    AABB aabb;
    aabb.center = glm::vec3(pos.x * 16 + 8, 128, pos.y * 16 + 8);
    aabb.extents = {8, 128, 8};

    if(aabb.IsInFrustrum(frustrum)) {
      auto p = mesh.get();
      float d = glm::distance(glm::vec2(camera.position.x, camera.position.z), glm::vec2(pos.x * 16 + 8, pos.y * 16 + 8));
      draw_list.push_back({d, p});
    }
  }

  std::sort(draw_list.begin(), draw_list.end(), [](auto& a, auto& b) {return a.first < b.first;});

  chunk_shader_.Use();
  atlas_.getTexture().Bind(0);
  vao_.Bind();

  for(const auto [_, mesh] : draw_list) {
    //mesh.second->Draw();
    mesh->Draw();
  }
}

const Atlas& WorldRenderer::GetAtlas() {
  return atlas_;
}
#include "world_renderer.h"
#include "atlas.h"
#include "chunk_mesh.h"
#include "fpv_camera.h"
#include "frustrum.h"
#include "game.h"
#include "world.h"
#include <algorithm>
#include <bgfx/bgfx.h>
#include <bgfx/defines.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <utility>
#include <vector>




WorldRenderer::WorldRenderer(World& world, const Atlas& atlas) : 
  world_(world),
  atlas_(atlas) {

  vertex_layout_.begin()
    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
    .add(bgfx::Attrib::Color0, 1, bgfx::AttribType::Float)
    .end();

  atlas_handle_ = bgfx::createUniform("s_atlas", bgfx::UniformType::Sampler);
  chunk_shader_ = Game::Get().GetShaderManager().LoadProgram("chunk_vs.sc", "chunk_fs.sc");
}

WorldRenderer::~WorldRenderer() {
  bgfx::destroy(chunk_shader_);
  bgfx::destroy(atlas_handle_);
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
      meshes_[chunk.first] = std::make_unique<ChunkMesh>(chunk.second, world_, atlas_, vertex_layout_);
    }
  }

  for(auto& it : meshes_) {
    auto& chunk = loaded_chunks.at(it.first);
    if(!chunk.HasChanged()) {continue;}

    meshes_[it.first] = std::make_unique<ChunkMesh>(chunk, world_, atlas_, vertex_layout_);
    chunk.Redrawn();
  }
  //update meshes
  // for(auto& mesh : meshes_) {
  //   mesh.second->Update();
  // }


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


  for(const auto [_, mesh] : draw_list) {
    bgfx::setState(BGFX_STATE_DEFAULT);
    bgfx::setTexture(0, atlas_handle_, atlas_.getTexture());
    mesh->Bind();
    bgfx::submit(0, chunk_shader_);
  }
}
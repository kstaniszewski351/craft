#include "world.h"
#include "chunk.h"
#include "ray.h"
#include <glm/common.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_int3.hpp>
#include <glm/geometric.hpp>

constexpr int RENDER_DISTANCE = 4;

int p_mod(int i) {
  return (i % 16 + 16) % 16;
}

int chunk_div(int p) {
  return (p >= 0) ? p / 16 : (p - 15) / 16;
  // if(p >= 0) {
  //   return p / 16;
  // }
  // else {
  //   return p / 16 - 1;
  // }
}

World::World() {
  // loaded_chunks_.emplace(glm::ivec2(0, 0), Chunk(glm::ivec2(0, 0), *this));
  // loaded_chunks_.emplace(glm::ivec2(1, 0), Chunk(glm::ivec2(1, 0), *this));
  LoadChunk(glm::ivec2(0, 0));
  // LoadChunk(glm::ivec2(1, 0));
}

std::unordered_map<glm::ivec2, Chunk>& World::GetChunks() {
  return loaded_chunks_;
}

void World::UpdateLoaded(glm::ivec3 player_pos) {
  glm::ivec2 player_chunk = GetChunkPos(player_pos);
  
  for(auto chunk = loaded_chunks_.cbegin(); chunk != loaded_chunks_.cend();) {
    float distance = glm::distance(glm::vec2(chunk->first), glm::vec2(player_chunk));
    if(distance > RENDER_DISTANCE) {
      UnloadChunk(chunk++->first);
    }
    else {
      chunk++;
    }
  }

  for(int x = -RENDER_DISTANCE; x <= RENDER_DISTANCE; x++) {
    for(int y = -RENDER_DISTANCE; y <= RENDER_DISTANCE; y++) {
      glm::ivec2 chunk = player_chunk + glm::ivec2(x, y);
      float distance = glm::distance(glm::vec2(chunk), glm::vec2(player_chunk));
      if(distance > RENDER_DISTANCE) {
        continue;
      }

      if(!loaded_chunks_.contains(chunk)) {
        LoadChunk(chunk);
      }
    }
  }
}

glm::ivec2 World::GetChunkPos(glm::ivec3 pos) {
  return glm::ivec2(chunk_div(pos.x), chunk_div(pos.z));
}

glm::ivec3 World::GetChunkBlockPos(glm::ivec3 pos) {
  return glm::ivec3(p_mod(pos.x), pos.y, p_mod(pos.z));
}

char World::GetBlock(glm::ivec3 pos) const {
  if(!InWorld(pos)) {
    return -1;
  }

  glm::ivec2 chunk_pos = GetChunkPos(pos);
  glm::ivec3 block_pos = GetChunkBlockPos(pos);


  auto chunk =  loaded_chunks_.find(chunk_pos);

  if(chunk == loaded_chunks_.end()) {
    return -1;
  }
  
  return chunk->second.GetBlock(block_pos);
}

bool World::SetBlock(glm::ivec3 pos, char block) {
  if(!InWorld(pos)) {
    return false;
  }

  glm::ivec2 chunk_pos = GetChunkPos(pos);
  glm::ivec3 block_pos = GetChunkBlockPos(pos);


  auto chunk =  loaded_chunks_.find(chunk_pos);

  if(chunk == loaded_chunks_.end()) {
    return false;
  }

  chunk->second.SetBlock(block, block_pos);
  return true;
}

bool World::InWorld(glm::ivec3 pos) {
  //glm::ivec2 chunk_pos = glm::ivec2(pos.x / 16, pos.z / 16);

  if(pos.y < 0 | pos.y > 255) {
    return false;
  }
  return true;
}

void World::LoadChunk(glm::ivec2 chunk_pos) {
  // if(loaded_chunks_.find(chunk_pos) != loaded_chunks_.end()) {
  //   return;
  // }
  loaded_chunks_.emplace(chunk_pos, Chunk(chunk_pos, *this));
  
  for(int i = 0; i < 4; i++) {
    if(loaded_chunks_.find(chunk_pos + CHUNK_DIRECTION_VECTORS[i]) != loaded_chunks_.end()) {
      loaded_chunks_.at(chunk_pos).SetNeighbor(&loaded_chunks_.at(chunk_pos + CHUNK_DIRECTION_VECTORS[i]), i);
      loaded_chunks_.at(chunk_pos + CHUNK_DIRECTION_VECTORS[i]).SetNeighbor(&loaded_chunks_.at(chunk_pos), (i + 2) % 4);
    }
  }
}

void World::UnloadChunk(glm::ivec2 chunk_pos) {
  for(int i = 0; i < 4; i++) {
    auto chunk = loaded_chunks_.find(chunk_pos + CHUNK_DIRECTION_VECTORS[i]);
    if(chunk != loaded_chunks_.end()) {
      chunk->second.SetNeighbor(nullptr, (i + 2) % 4);
    }
  }
  loaded_chunks_.erase(chunk_pos);
}

float PlaneIntersect(float o, float d) {
  float c = d >= 0 ? glm::ceil(o) : glm::floor(o);

  return (c - o) / d;
  // if(d > 0) {
  //   return (o + 1 - d) / d;
  // }
}

std::optional<RaycastResult> World::Raycast(Ray ray) {
  glm::ivec3 step;
  step.x = ray.dir.x >= 0 ? glm::ceil(ray.dir.x) : glm::floor(ray.dir.x);
  step.y = ray.dir.y >= 0 ? glm::ceil(ray.dir.y) : glm::floor(ray.dir.y);
  step.z = ray.dir.z >= 0 ? glm::ceil(ray.dir.z) : glm::floor(ray.dir.z);
  glm::ivec3 ipos = glm::floor(ray.pos);
  glm::vec3 t_max;
  t_max.x = PlaneIntersect(ray.pos.x, ray.dir.x);
  t_max.y = PlaneIntersect(ray.pos.y, ray.dir.y);
  t_max.z = PlaneIntersect(ray.pos.z, ray.dir.z);

  glm::vec3 t_delta = glm::abs(glm::vec3(1.0) / ray.dir);
  glm::ivec3 place_dir = glm::ivec3(0);

  while(true) {
    if(GetBlock(ipos) != 0) {
      return RaycastResult({
        .pos = ipos,
        .face = place_dir
      });
    }

    if(t_max.x < t_max.y) {
      if(t_max.x < t_max.z) {
        if(t_max.x > ray.length) {
          break;
        }
        t_max.x += t_delta.x;
        ipos.x += step.x;
        place_dir = glm::ivec3(-step.x, 0, 0);
      }
      else {
        if(t_max.z > ray.length) {
          break;
        }
        t_max.z += t_delta.z;
        ipos.z += step.z;
        place_dir = glm::ivec3(0, 0, -step.z);
      }
    }
    else {
      if(t_max.y < t_max.z) {
        if(t_max.y > ray.length) {
          break;
        }
        t_max.y += t_delta.y;
        ipos.y += step.y;
        place_dir = glm::ivec3(0, -step.y, 0);
      }
      else {
        if(t_max.z > ray.length) {
          break;
        }
        t_max.z += t_delta.z;
        ipos.z += step.z;
        place_dir = glm::ivec3(0, 0, -step.z);
      }
    }


  }
  return {};

  //return glm::ivec3(0);
};
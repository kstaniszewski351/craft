#include "world.h"
#include "chunk.h"
#include "direction.h"
#include "registries.h"
#include "ray.h"
#include <glm/common.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_int3.hpp>
#include <glm/geometric.hpp>
#include <queue>
#include <unordered_set>

constexpr int RENDER_DISTANCE = 4;

int p_mod(int i) {
  return (i % 16 + 16) % 16;
}

int chunk_div(int p) {
  return (p >= 0) ? p / 16 : (p - 15) / 16;
}

World::World() {

}

std::unordered_map<glm::ivec2, Chunk>& World::GetChunks() {
  return loaded_chunks_;
}

void World::Update(glm::ivec3 player_pos) {
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

  for(SkyLightUpdate& update  : sky_light_updates_) {
    if(update.height_change) {
      unsigned char height = GetHeight({update.pos.x, update.pos.z});
      glm::ivec3 pos = {update.pos.x, height, update.pos.z};
      fillSkyLightRemove(pos, true);
      fillSkyLightAdd(pos, 15, true);
    }
    fillSkyLightRemove(update.pos, false);   
  }
  for(LightUpdate& update : sky_light_add_updates_) {
    fillSkyLightAdd(update.pos, update.level, false);
  }
  for(LightUpdate& update : light_remove_updates_) {
    fillBlockLightRemove(update.pos, update.level);
  }

  for(LightUpdate& update : light_add_updates_) {
    fillBlockLightAdd(update.pos, update.level);
  }
  light_remove_updates_.clear();
  light_add_updates_.clear();
  sky_light_updates_.clear();
  sky_light_add_updates_.clear();
}

void World::fillBlockLightAdd(glm::ivec3 pos, char level) {
  std::queue<std::pair<glm::ivec3, char>> to_check;
  std::unordered_set<glm::ivec3> visited;

  to_check.push({pos, level});
  visited.insert(pos);
  if(level > GetBlockLightLevel(pos)) {
    SetBlockLightLevel(pos, level);
  }

  while(to_check.size() != 0) {
    auto top = to_check.front();
    to_check.pop();
    glm::ivec3 check_pos = top.first;
    char check_level = top.second;

    for(int i = 0; i < DirectionCount; i++) {
      glm::ivec3 new_pos = check_pos + DIRECTION_VECTORS_I[i];
      if(
        !visited.contains(new_pos) &&
        GetBlock(new_pos) == 0 &&
        GetBlockLightLevel(new_pos) < check_level
      ) {
        visited.insert(new_pos);
        if(check_level > 1) {
          to_check.push({new_pos, check_level -1});
        }
        SetBlockLightLevel(new_pos, check_level - 1);
      }
    }
  }

}

void World::fillBlockLightRemove(glm::ivec3 pos, char level) {
  std::queue<std::pair<glm::ivec3, char>> to_check;
  std::unordered_set<glm::ivec3> visited;

  to_check.push({pos, level});
  visited.insert(pos);
  SetBlockLightLevel(pos, 0);

  while(to_check.size() != 0) {
    auto top = to_check.front();
    to_check.pop();
    glm::ivec3 check_pos = top.first;
    char check_level = top.second;

    for(int i = 0; i < DirectionCount; i++) {
      glm::ivec3 new_pos = check_pos + DIRECTION_VECTORS_I[i];
      if(
        !visited.contains(new_pos) &&
        GetBlockLightLevel(new_pos) != 0
      ) {
        char block_level = GetBlockLightLevel(new_pos);
        visited.insert(new_pos);
        if(block_level == check_level - 1) {
          if(check_level > 1) {
            to_check.push({new_pos, check_level -1});
          }
          SetBlockLightLevel(new_pos, 0);
        }
        else if(block_level >= check_level) {
          light_add_updates_.push_back({new_pos, static_cast<char>(block_level) });
        }
      }
    }
  }
}

void World::fillSkyLightRemove(glm::ivec3 pos, bool column) {
  std::queue<std::pair<glm::ivec3, char>> to_check;
  std::unordered_set<glm::ivec3> visited;


  if(column) {
    for(int i = pos.y - 1; i >=0 && GetBlock({pos.x, i, pos.z}) == 0; i--) {
      glm::ivec3 new_pos = {pos.x, i, pos.z};
      to_check.push({new_pos, GetSkyLightLevel(new_pos)});
      visited.insert(new_pos);
      SetSkyLightLevel(new_pos, 0);
    }
  }
  else {
    visited.insert(pos);
    to_check.push({pos, GetSkyLightLevel(pos)});
    SetSkyLightLevel(pos, 0);
  }

  while(to_check.size() != 0) {
    auto top = to_check.front();
    to_check.pop();
    glm::ivec3 check_pos = top.first;
    char check_level = top.second;

    for(int i = 0; i < DirectionCount; i++) {
      glm::ivec3 new_pos = check_pos + DIRECTION_VECTORS_I[i];
      if(
        !visited.contains(new_pos) &&
        GetBlock(new_pos) == 0 &&
        GetSkyLightLevel(new_pos) != 0
      ) {
        char block_level = GetSkyLightLevel(new_pos);
        visited.insert(new_pos);
        if(block_level == check_level - 1) {
          if(check_level > 1) {
            to_check.push({new_pos, check_level -1});
          }
          SetSkyLightLevel(new_pos, 0);
        }
        else {
          sky_light_add_updates_.push_back({new_pos, static_cast<char>(block_level) });
        }
      }
    }
  }
}

void World::fillSkyLightAdd(glm::ivec3 pos, char level, bool column) {
  std::queue<std::pair<glm::ivec3, char>> to_check;
  std::unordered_set<glm::ivec3> visited;

  if(GetBlock(pos) == 0) {
    to_check.push({pos, level});
    visited.insert(pos);
  }
  if(level > GetSkyLightLevel(pos)) {
    SetSkyLightLevel(pos, level);
  }
  if(column) {
    for(int y = pos.y + 1; y < CHUNK_SIZE.y; y++) {
      glm::ivec3 check_pos = {pos.x, y, pos.z};
      
      to_check.push({check_pos, level});
      if(level > GetSkyLightLevel(pos)) {
        SetSkyLightLevel(pos, level);
      }
    }
  }
  while(to_check.size() != 0) {
    auto top = to_check.front();
    to_check.pop();
    glm::ivec3 check_pos = top.first;
    char check_level = top.second;

    for(int i = 0; i < DirectionCount; i++) {
      glm::ivec3 new_pos = check_pos + DIRECTION_VECTORS_I[i];
      if(
        !visited.contains(new_pos) &&
        GetSkyLightLevel(new_pos) < check_level
        && GetBlock(new_pos) == 0
      ) {
        visited.insert(new_pos);
        if(check_level > 1) {
          to_check.push({new_pos, check_level -1});
        }
        SetSkyLightLevel(new_pos, check_level - 1);
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

char World::GetBlockLightLevel(glm::ivec3 pos) const {
  if(!InWorld(pos)) {
    return -1;
  }

  glm::ivec2 chunk_pos = GetChunkPos(pos);
  glm::ivec3 block_pos = GetChunkBlockPos(pos);


  auto chunk =  loaded_chunks_.find(chunk_pos);

  if(chunk == loaded_chunks_.end()) {
    return -1;
  }
  
  return chunk->second.GetBlockLightLevel(block_pos);
}

char World::GetSkyLightLevel(glm::ivec3 pos) const {
  if(!InWorld(pos)) {
    return -1;
  }

  glm::ivec2 chunk_pos = GetChunkPos(pos);
  glm::ivec3 block_pos = GetChunkBlockPos(pos);


  auto chunk =  loaded_chunks_.find(chunk_pos);

  if(chunk == loaded_chunks_.end()) {
    return -1;
  }
  
  return chunk->second.GetSkyLightLevel(block_pos);
}

unsigned char World::GetHeight(glm::ivec2 pos) const {

  glm::ivec2 chunk_pos = GetChunkPos({pos.x, 0, pos.y});
  glm::ivec3 block_pos = GetChunkBlockPos({pos.x, 0, pos.y});


  auto chunk =  loaded_chunks_.find(chunk_pos);
  if(chunk == loaded_chunks_.end()) {
    return -1;
  }

  return chunk->second.GetHeight({block_pos.x, block_pos.z});
}

bool World::SetBlockLightLevel(glm::ivec3 pos, char level) {
  if(!InWorld(pos)) {
    return false;
  }

  glm::ivec2 chunk_pos = GetChunkPos(pos);
  glm::ivec3 block_pos = GetChunkBlockPos(pos);


  auto chunk =  loaded_chunks_.find(chunk_pos);

  if(chunk == loaded_chunks_.end()) {
    return false;
  }

  chunk->second.SetBlockLightLevel(block_pos, level);
  return true;
}

bool World::SetSkyLightLevel(glm::ivec3 pos, char level) {
  if(!InWorld(pos)) {
    return false;
  }

  glm::ivec2 chunk_pos = GetChunkPos(pos);
  glm::ivec3 block_pos = GetChunkBlockPos(pos);


  auto chunk =  loaded_chunks_.find(chunk_pos);

  if(chunk == loaded_chunks_.end()) {
    return false;
  }

  chunk->second.SetSkyLightLevel(block_pos, level);
  return true;
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

  char level = GetBlockLightLevel(pos);
  light_remove_updates_.push_back({pos, level});
  unsigned char height = chunk->second.GetHeight({block_pos.x, block_pos.z});

  chunk->second.SetBlock(block, block_pos);
  bool height_changed = false;
  if(chunk->second.GetHeight({block_pos.x, block_pos.z}) != height) {
    height_changed = true;
  }
  sky_light_updates_.push_back({pos, height_changed});
  if(block != 0) {
    char level = gBlockRegistry.Get(block).GetProps().lightLevel;
    light_add_updates_.push_back({pos, level});
  }
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
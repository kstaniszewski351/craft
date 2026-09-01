#pragma once
#include "ray.h"
#include <glm/ext/vector_int3.hpp>
#include <optional>
#include <queue>
#include <unordered_set>
#define GLM_ENABLE_EXPERIMENTAL

#include <unordered_map>
#include <glm/gtx/hash.hpp>

class Chunk;

struct RaycastResult {
  glm::vec3 pos;
  glm::vec3 face;
};

class World {
  public:
    World();
    bool SetBlock(glm::ivec3 pos, char block);
    char GetBlock(glm::ivec3 pos) const;
    char GetBlockLightLevel(glm::ivec3 pos) const;
    char GetSkyLightLevel(glm::ivec3 pos) const;
    unsigned char GetHeight(glm::ivec2 pos) const;
    bool SetBlockLightLevel(glm::ivec3 pos, char level);
    bool SetSkyLightLevel(glm::ivec3 pos, char level);
    static bool InWorld(glm::ivec3 pos);
    void Update(glm::ivec3 player_pos);
    void LoadChunk(glm::ivec2 chunk_pos);
    void UnloadChunk(glm::ivec2 chunk_pos);
    static glm::ivec2 GetChunkPos(glm::ivec3 pos);
    static glm::ivec3 GetChunkBlockPos(glm::ivec3 pos);
    std::optional<RaycastResult> Raycast(Ray ray);
    std::unordered_map<glm::ivec2, Chunk>& GetChunks();
  private:
    struct LightUpdate {
      glm::ivec3 pos;
      char level;
    };
    struct SkyLightUpdate {
      glm::ivec3 pos;
      bool height_change;
      char prev_level;
    };

    void fillBlockLightAdd(glm::ivec3 pos, char level);
    void fillBlockLightRemove(glm::ivec3 pos, char level);
    void fillSkyLightAdd(glm::ivec3 pos, char level, bool column);
    void fillSkyLightRemove(glm::ivec3 pos, bool column);
    std::unordered_map<glm::ivec2, Chunk> loaded_chunks_;
    std::vector<LightUpdate> light_add_updates_;
    std::vector<LightUpdate> light_remove_updates_;
    std::vector<SkyLightUpdate> sky_light_updates_;
    std::vector<LightUpdate> sky_light_add_updates_;
};
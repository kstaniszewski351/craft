#pragma once
#include <glm/ext/vector_int3.hpp>
#include <optional>
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
    static bool InWorld(glm::ivec3 pos);
    void UpdateLoaded(glm::ivec3 player_pos);
    // char GetBlock(glm::ivec2 chunk_pos, glm::ivec3 block_pos) const;
    void LoadChunk(glm::ivec2 chunk_pos);
    void UnloadChunk(glm::ivec2 chunk_pos);
    static glm::ivec2 GetChunkPos(glm::ivec3 pos);
    static glm::ivec3 GetChunkBlockPos(glm::ivec3 pos);
    std::optional<RaycastResult> Raycast(glm::vec3 pos, glm::vec3 dir, float max_lenght);
    std::unordered_map<glm::ivec2, Chunk>& GetChunks();
  private:
    //char* getBlockPtr(glm::ivec3 pos);
    //char* getBlockPtr(glm::ivec2 chunk_pos, glm::ivec3 block_pos);
    std::unordered_map<glm::ivec2, Chunk> loaded_chunks_;
};
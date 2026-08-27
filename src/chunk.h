#pragma once

#include "direction.h"
#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>

class World;

constexpr glm::ivec3 CHUNK_SIZE = {16, 256, 16};

using Blocks = std::array<std::array<std::array<char, CHUNK_SIZE.x>, CHUNK_SIZE.y>, CHUNK_SIZE.z>;

enum ChunkDirection {
  North = 0,
  East ,
  South,
  West
};

constexpr std::array<glm::ivec2, 4> CHUNK_DIRECTION_VECTORS = {
  glm::ivec2(0, -1),
  glm::ivec2(1, 0),
  glm::ivec2(0, 1),
  glm::ivec2(-1, 0)
};

class Chunk {
  public:
    Chunk(glm::ivec2 pos, World& world);
    void SetBlock(char block, glm::ivec3 pos);
    char GetBlock(glm::ivec3 pos) const;
    char GetBlockLightLevel(glm::ivec3 pos) const;
    char GetSkyLightLevel(glm::ivec3 pos) const;
    void SetBlockLightLevel(glm::ivec3 pos, char level);
    void SetSkyLightLevel(glm::ivec3 pos, char level);
    const Blocks& GetBlocks() const;
    bool HasChanged() const;
    glm::ivec2 GetPos() const;
    void SetNeighbor(Chunk* neighbor, int index);
    Chunk* GetNeighbor(int direction) const;
    void Redrawn();
    std::array<bool, 6> GetVisibleFaces(glm::ivec3 pos) const;
  private:
    std::array<Chunk*, 4> neighbors_;
    glm::ivec2 chunk_pos_;
    bool has_changed_;
    std::unique_ptr<Blocks> blocks_;
    std::unique_ptr<Blocks> lighting_;
    World& world_;
    
};
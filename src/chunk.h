#pragma once

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>

class World;



using Blocks = std::array<std::array<std::array<char, 16>, 256>, 16>;

enum ChunkDirection {
  North = 0,
  East ,
  South,
  West
};

constexpr std::array<glm::ivec2, 4> ChunkDirectionVectors = {
  glm::ivec2(0, -1), // North
  glm::ivec2(1, 0),  // East
  glm::ivec2(0, 1),  // South
  glm::ivec2(-1, 0)  // West
};

class Chunk {
  public:
    Chunk(glm::ivec2 pos, World& world);
    void SetBlock(char block, glm::ivec3 pos);
    char GetBlock(glm::ivec3 pos) const;
    const Blocks& GetBlocks() const;
    bool HasChanged() const;
    glm::ivec2 GetPos() const;
    void SetNeighbor(Chunk* neighbor, int index);
    Chunk* GetNeighbor(int direction) const;
    void Redrawn();
  private:
    std::array<Chunk*, 4> neighbors_;
    glm::ivec2 chunk_pos_;
    bool has_changed_;
    std::unique_ptr<Blocks> blocks_;
    World& world_;
    
};
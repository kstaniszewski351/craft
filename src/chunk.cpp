#include "chunk.h"
#include "chunk_gen.h"
#include "direction.h"
#include "world.h"
#include <array>
#include <memory>



Chunk::Chunk(glm::ivec2 pos, World& world) : 
  chunk_pos_(pos),
  has_changed_(false),
  blocks_(std::make_unique<Blocks>()),
  lighting_(std::make_unique<Blocks>()),
  heightmap_(std::make_unique<std::array<std::array<unsigned char, CHUNK_SIZE.z>, CHUNK_SIZE.x>>()),
  world_(world) {

  neighbors_.fill(nullptr);
  
  Generate(*this);
  createHeightmap();
}

char Chunk::GetBlockLightLevel(glm::ivec3 pos) const {
  return lighting_->data()[pos.x][pos.y][pos.z] & 0x0f;
};

char Chunk::GetSkyLightLevel(glm::ivec3 pos) const {
  return (lighting_->data()[pos.x][pos.y][pos.z] >> 4) & 0x0f;
};

void Chunk::SetBlockLightLevel(glm::ivec3 pos, char level) {
  char& ref = lighting_->data()[pos.x][pos.y][pos.z];
  ref = level | (ref & 0xf0);
  has_changed_ = true;
}

void Chunk::SetSkyLightLevel(glm::ivec3 pos, char level) {
  char& ref = lighting_->data()[pos.x][pos.y][pos.z];
  ref = (level << 4) | (ref & 0x0f);
  has_changed_ = true;
}

void Chunk::SetBlock(char block, glm::ivec3 pos) {
  blocks_->data()[pos.x][pos.y][pos.z] = block;
  has_changed_ = true;

  if(pos.y > heightmap_->data()[pos.x][pos.z] && block != 0) {
    heightmap_->data()[pos.x][pos.z] = pos.y;
  }
  if(pos.y == heightmap_->data()[pos.x][pos.z] && block == 0) {
    int y = pos.y;

    while(blocks_->data()[pos.x][y][pos.z] == 0 && y > 0) {
      y--;
    }

    heightmap_->data()[pos.x][pos.z] = y;
  }
}

unsigned char Chunk::GetHeight(glm::ivec2 pos) const {
  return heightmap_->data()[pos.x][pos.y];
}

char Chunk::GetBlock(glm::ivec3 pos) const {
  return blocks_->data()[pos.x][pos.y][pos.z];
}

bool Chunk::HasChanged() const {
  return has_changed_;
}

void Chunk::Redrawn() {
  has_changed_ = false;
}

glm::ivec2 Chunk::GetPos() const {
  return chunk_pos_;
}

const Blocks& Chunk::GetBlocks() const {
  return *blocks_;
}


void Chunk::SetNeighbor(Chunk* neighbor, int index) {
  neighbors_[index] = neighbor;
  has_changed_ = true;
};

Chunk* Chunk::GetNeighbor(int direction) const {
  return neighbors_[direction];
}

std::array<bool, 6> Chunk::GetVisibleFaces(glm::ivec3 pos) const {
  std::array<bool, 6> res = {};
  if(pos.y == 15 || blocks_->data()[pos.x][pos.y+1][pos.z] == 0) {
    res[Up] = true;
  }
  if(pos.y == 0 || blocks_->data()[pos.x][pos.y-1][pos.z] == 0) {
    res[Down] = true;
  }
  if(
    (pos.x == 0 && neighbors_[West] != nullptr && neighbors_[West]->GetBlocks()[15][pos.y][pos.z] == 0) ||
    (pos.x > 0 && blocks_->data()[pos.x-1][pos.y][pos.z] == 0)
  ) {
    res[Left] = true;
  }
  if(
    (pos.x == 15 && neighbors_[East] != nullptr && neighbors_[East]->GetBlocks()[0][pos.y][pos.z] == 0) ||
    (pos.x < 15 && blocks_->data()[pos.x+1][pos.y][pos.z] == 0)
  ) {
    res[Right] = true;
  }
  if(
    (pos.z == 0 && neighbors_[North] != nullptr && neighbors_[North]->GetBlocks()[pos.x][pos.y][15] == 0) ||
    (pos.z > 0 && blocks_->data()[pos.x][pos.y][pos.z-1] == 0)
  ) {
    res[Front] = true;
  }
  if(
    (pos.z == 15 && neighbors_[South] != nullptr && neighbors_[South]->GetBlocks()[pos.x][pos.y][0] == 0) ||
    (pos.z < 15 && blocks_->data()[pos.x][pos.y][pos.z+1] == 0)
  ) {
    res[Back] = true;
  }


  return res;
};

void Chunk::createHeightmap() {
  for(int x = 0; x < CHUNK_SIZE.x; x++) {
    for(int z = 0; z < CHUNK_SIZE.z; z++) {
      int y = CHUNK_SIZE.y - 1;
      while(blocks_->data()[x][y][z] == 0 && y > 0) {
        y--;
      }
      heightmap_->data()[x][z] = y;
    }
  }
}
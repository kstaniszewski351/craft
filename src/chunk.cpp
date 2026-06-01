#include "chunk.h"
#include "chunk_gen.h"
#include "world.h"
#include <array>
#include <memory>



Chunk::Chunk(glm::ivec2 pos, World& world) : 
  chunk_pos_(pos),
  has_changed_(true),
  blocks_(std::make_unique<Blocks>()),
  world_(world) {

  neighbors_.fill(nullptr);
  
  Generate(*this);
}

void Chunk::SetBlock(char block, glm::ivec3 pos) {
  blocks_->data()[pos.x][pos.y][pos.z] = block;

  has_changed_ = true;
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
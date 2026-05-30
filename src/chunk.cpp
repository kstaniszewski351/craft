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
  
  // for(int x = 0; x < 16; x++) {
  //   for(int y = 0; y < 256; y++) {
  //     for(int z = 0; z < 16; z++) {
  //       auto block = &blocks_->data()[x][y][z];

  //       if(y == 4) {
  //         *block = 2;
  //       }
  //       else if(y < 4) {
  //         *block = 1;
  //       }
  //       else {
  //         *block = 0;
  //       }
  //     }
  //   }
  // }
  Generate(*this);
}


bool Chunk::InChunk(glm::ivec3 pos) {
  // if(pos.x > 15 | pos.x < 0 | pos.y < 0 | pos.y > 15 | pos.z < 0 | pos.z > 15) {
  //   return false;
  // }
  return true;
}

void Chunk::SetBlock(char block, glm::ivec3 pos) {
  // if(!InChunk(pos)) {
  //   return;
  // }
  blocks_->data()[pos.x][pos.y][pos.z] = block;

  has_changed_ = true;
}

char Chunk::GetBlock(glm::ivec3 pos) const {
  // if(!InChunk(pos)) {
  //   return -1;
  // }
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
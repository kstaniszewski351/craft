#pragma once

#include "atlas.h"
#include "direction.h"
#include <array>
#include <string>
#include <string_view>
#include <unordered_map>
class Block;


// class Block  {
//   Block(char id, std::string texture);
//   Block(char id, std::array<std::string, 6> textures);
// };

// struct Block {
//   std::array<int, 6> faces;
// };

// class BlockReg {
//  public:
//   BlockReg(Atlas atlas, std::unordered_map<char, Block> blocks);
//   const Atlas& GetAtlas();
//   const Block& GetBlock(char id);
//  private:
//   std::unordered_map<char, Block> blocks_;
//   Atlas atlas_;
// };

// class BlockRegBuilder {
//  public:
//   void AddBlock(char id, std::array<std::string, 6> textures);
//   BlockReg Build();
//  private:
//   std::unordered_map<char, Block> blocks_;
//   std::unordered_map<std::string, char> texture_indices_;
//   std::vector<Bitmap> bitmaps_;
// };
class AtlasBuilder {
 public:
  int AddTexture(std::string path);
  Atlas Build();
 private:
  std::vector<Bitmap> bitmaps_;
};


// class SidedBlock : BaseBlock {
//   SidedBlock(char id, std::array<std::string, 6> sides);
// };

// constexpr std::array<Block, 4> BLOCKS = {
//   Block {1, ""}
// };
#include "atlas_builder.h"
#include "atlas.h"
#include <unordered_map>

// BlockReg::BlockReg(Atlas atlas, std::unordered_map<char, Block> blocks) :
// atlas_(atlas),  blocks_(blocks){

// }

// const Atlas& BlockReg::GetAtlas() {
//   return atlas_;
// }

// const Block& BlockReg::GetBlock(char id) {
//   return blocks_.at(id);
// }

// void BlockRegBuilder::AddBlock(char id, std::array<std::string, 6> textures) {

//   std::array<int, 6> faces;

//   for(int i = 0; i < 6; i++) {
//     auto& texture_path = textures[i];
//     if(texture_indices_.contains(texture_path)) {continue;}

//     Bitmap bitmap = Bitmap(texture_path, PixelFormat::RGB, PixelType::UInt8);
//     bitmaps_.push_back(std::move(bitmap));

//     int index = bitmaps_.size() - 1;
//     texture_indices_[texture_path] = index;
//     faces[i] = index;
//   }

//   blocks_[id] = {faces};
// }

// BlockReg BlockRegBuilder::Build() {
//   Atlas atlas = Atlas(bitmaps_, 16);

  
// }

int AtlasBuilder::AddTexture(std::string path) {
  Bitmap bitmap = Bitmap(path, PixelFormat::RGB, PixelType::UInt8);
  bitmaps_.push_back(std::move(bitmap));
  return bitmaps_.size()-1;
}

Atlas AtlasBuilder::Build() {
  Atlas atlas = Atlas(bitmaps_, 16);

  return atlas;
}
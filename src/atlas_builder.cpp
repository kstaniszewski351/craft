#include "atlas_builder.h"
#include "atlas.h"
#include <unordered_map>


int AtlasBuilder::AddTexture(std::string path) {
  Bitmap bitmap = Bitmap(path, PixelFormat::RGBA, PixelType::UInt8);
  bitmaps_.push_back(std::move(bitmap));
  return bitmaps_.size()-1;
}

Atlas AtlasBuilder::Build() {
  Atlas atlas = Atlas(bitmaps_, 16);

  return atlas;
}
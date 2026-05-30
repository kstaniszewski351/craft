#include "block.h"
#include "atlas_builder.h"
#include "direction.h"

UniformBlock::UniformBlock(std::string texture) : texture_path(texture) {

}

void UniformBlock::RegisterTextures(AtlasBuilder& builder) {
  texture_id = builder.AddTexture(texture_path);
}

int UniformBlock::GetTexture(Direction dir) const {
  return texture_id;
}

LogBlock::LogBlock(std::string top, std::string sides) :
  top_path(top),
  side_path(sides) {
}

void LogBlock::RegisterTextures(AtlasBuilder& builder) {
  top_id = builder.AddTexture(top_path);
  side_id = builder.AddTexture(side_path);
}

int LogBlock::GetTexture(Direction dir) const {
  if(dir == Up || dir == Down) {
    return top_id;
  }
  else {
    return side_id;
  }
}
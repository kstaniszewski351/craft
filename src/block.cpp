#include "block.h"
#include "atlas_builder.h"
#include "direction.h"

Block::Block(Properties props) :
  props_(props) {

}

const Block::Properties& Block::GetProps() const {
  return props_;
}

UniformBlock::UniformBlock(std::string texture, bool random_rotate, Properties props) :
  Block(props),
  texture_path_(texture),
  random_rotate_(random_rotate) {

}

void UniformBlock::RegisterTextures(AtlasBuilder& builder) {
  texture_id_ = builder.AddTexture(texture_path_);
}

int UniformBlock::GetTexture(Direction dir) const {
  return texture_id_;
}

bool UniformBlock::RandomRotate(Direction dir) const {
  return random_rotate_;
}

LogBlock::LogBlock(std::string top, std::string sides, Properties props) :
  Block(props),
  top_path_(top),
  side_path_(sides) {
}

void LogBlock::RegisterTextures(AtlasBuilder& builder) {
  top_id_ = builder.AddTexture(top_path_);
  side_id_ = builder.AddTexture(side_path_);
}

int LogBlock::GetTexture(Direction dir) const {
  if(dir == Up || dir == Down) {
    return top_id_;
  }
  else {
    return side_id_;
  }
}

GrassBlock::GrassBlock(std::string top, std::string bottom, std::string sides, Properties props) :
  Block(props),
  top_path_(top),
  bottom_path_(bottom),
  side_path_(sides) {

 }

 void GrassBlock::RegisterTextures(AtlasBuilder& builder) {
  top_id_ = builder.AddTexture(top_path_);
  bottom_id_ = builder.AddTexture(bottom_path_);
  side_id_ = builder.AddTexture(side_path_);
 }

 int GrassBlock::GetTexture(Direction dir) const {
  if(dir == Up) {
    return top_id_;
  }
  else if(dir == Down) {
    return bottom_id_;
  }
  return side_id_;
}

bool GrassBlock::RandomRotate(Direction dir) const {
  if(dir == Up) {
    return true;
  }
  return false;
}
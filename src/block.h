#pragma once

#include "atlas_builder.h"
#include "direction.h"
#include <string>

class Block {
 public:
  virtual void RegisterTextures(AtlasBuilder& builder) {};
  virtual int GetTexture(Direction dir) const = 0;
  virtual ~Block() {};
};

class UniformBlock : public Block {
 public:
  UniformBlock(std::string texture);
  void RegisterTextures(AtlasBuilder& builder) override;
  int GetTexture(Direction dir) const override;
 private:

  int texture_id;
  std::string texture_path;
};

class LogBlock : public Block {
 public:
  LogBlock(std::string top, std::string sides);

  void RegisterTextures(AtlasBuilder& builder) override;
  int GetTexture(Direction dir) const override;
 private:
  int top_id;
  int side_id;
  std::string top_path;
  std::string side_path;
};
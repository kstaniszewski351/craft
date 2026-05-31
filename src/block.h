#pragma once

#include "atlas_builder.h"
#include "direction.h"
#include <glm/ext/vector_float3.hpp>
#include <string>

class Block {
 public:
  virtual void RegisterTextures(AtlasBuilder& builder) {};
  virtual int GetTexture(Direction dir) const = 0;
  virtual bool RandomRotate(Direction dir) const {
    return false;
  };
  //virtual ~Block() {};
};

class UniformBlock : public Block {
 public:
  UniformBlock(std::string texture);
  void RegisterTextures(AtlasBuilder& builder) override;
  int GetTexture(Direction dir) const override;
  bool RandomRotate(Direction dir) const override;
 private:

  int texture_id_;
  std::string texture_path_;
};

class LogBlock : public Block {
 public:
  LogBlock(std::string top, std::string sides);
  void RegisterTextures(AtlasBuilder& builder) override;
  int GetTexture(Direction dir) const override;
 private:
  int top_id_;
  int side_id_;
  std::string top_path_;
  std::string side_path_;
};

class GrassBlock : public Block {
 public:
  GrassBlock(std::string top, std::string bottom, std::string sides);
  void RegisterTextures(AtlasBuilder& builder) override;
  int GetTexture(Direction dir) const override;
  bool RandomRotate(Direction dir) const override;
 private:
  int top_id_;
  int bottom_id_;
  int side_id_;
  std::string top_path_;
  std::string bottom_path_;
  std::string side_path_;
};
#pragma once

#include "bgfx/bgfx.h"
#include "entity.h"
#include <string>
#include "block.h"
#include "registry.h"

class World;

struct ItemTexInfo {
  bgfx::TextureHandle texture;
  glm::vec2 uv_offset;
  glm::vec2 uv_scale;
};

class Item : public Registrable {
 public:
  Item(std::string name);
  virtual void OnUse(World& world, Entity player) const {};
  virtual ItemTexInfo GetTexInfo() const = 0;
 private:
  std::string name_;
};

class BlockItem : public Item {
 public:
  BlockItem(std::string name, const Block* block);
  void OnUse(World& world, Entity player) const override;
  void RegisterTex(std::vector<const Block*>& blocks);
  ItemTexInfo GetTexInfo() const override;
 private:
  int atlas_id_;
  const Block* block_;
  ItemTexInfo tex_info_;
};
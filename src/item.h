#pragma once

#include "entity.h"
#include <string>
#include "block.h"
#include "registry.h"

class World;

class Item : public Registrable {
 public:
  Item(std::string name);
  virtual void OnUse(World& world, Entity player) const {};
 private:
  std::string name_;
};

class BlockItem : public Item {
 public:
  BlockItem(std::string name, const Block& block);
  void OnUse(World& world, Entity player) const;
 private:
  const Block& block_;
};
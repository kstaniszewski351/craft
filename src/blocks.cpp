#include "blocks.h"
#include "block.h"
#include "item.h"
#include "registries.h"
#include <memory>
#include <utility>
std::vector<const BlockItem*> block_items;

const Block& Register(std::string name, Block* block) {

  return gBlockRegistry.Register(name, std::unique_ptr<Block>(block));
}

const Block& RegisterWithItem(std::string name, Block* block, std::string display_name) {
  const Block& block_ref = Register(name, block);
  auto item_ptr = std::make_unique<BlockItem>(name, block_ref);
  const BlockItem& item_ref = *item_ptr.get();
  gItemRegistry.Register(name, std::move(item_ptr));
  //block_items.push_back(item_ref);
  return block_ref;
}



void RegisterBlocks() {
  RegisterWithItem(
    "dirt",
    new UniformBlock("res/textures/dirt.png"),
    "Dirt"
  );
  RegisterWithItem(
    "grass", 
    new GrassBlock("res/textures/grass_top.png", "res/textures/dirt.png", "res/textures/grass_side.png"),
    "Grass block"
  );
}
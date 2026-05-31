#include "block_registry.h"
#include "atlas.h"
#include "atlas_builder.h"
#include "block.h"
#include <memory>

void BlockRegistry::Register(std::string name, std::unique_ptr<Block> block) {
  char id = next_id;
  if(name_to_id_.contains(name)) {
    return;
  }
  blocks_[id] = std::move(block);
  name_to_id_[name] = id;
  next_id ++;
}

Atlas BlockRegistry::BuildAtlas() {
  AtlasBuilder builder;

  for(auto& [_, block] : blocks_) {
    block->RegisterTextures(builder);
  }
  return builder.Build();
}

char BlockRegistry::GetID(std::string name) const {
  return name_to_id_.at(name);
}

const Block& BlockRegistry::GetBlock(char id) const {
  auto& f = blocks_.at(id);
  auto p = f.get();
  return *p;
}

const Block& BlockRegistry::GetBlock(std::string name) const {
  return GetBlock(GetID(name));
}

void RegisterBlocks() {
  gBlockRegistry.Register("dirt", std::make_unique<UniformBlock>(
   "res/textures/dirt.png"
  ));
  gBlockRegistry.Register("log", std::make_unique<LogBlock>(
    "res/textures/log_top.png", "res/textures/log_side.png"
  ));
  gBlockRegistry.Register("grass", std::make_unique<GrassBlock>(
    "res/textures/grass_top.png",
    "res/textures/dirt.png",
    "res/textures/grass_side.png"
  ));
}

BlockRegistry gBlockRegistry;
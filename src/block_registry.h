#pragma once

#include "block.h"
class BlockRegistry {
 public:
  void Register(std::string name, std::unique_ptr<Block> block);
  Atlas BuildAtlas();
  char GetID(std::string name) const;
  const Block& GetBlock(char id) const;
  const Block& GetBlock(std::string name) const;
 private:
  std::unordered_map<char, std::unique_ptr<Block>> blocks_;
  std::unordered_map<std::string, char> name_to_id_;
  char next_id = 1;
};

void RegisterBlocks();

extern BlockRegistry gBlockRegistry;
#pragma once

#include "atlas.h"
#include <string>
class Block;

class AtlasBuilder {
 public:
  int AddTexture(std::string path);
  Atlas Build();
 private:
  std::vector<Bitmap> bitmaps_;
};
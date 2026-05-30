#pragma once

#include "bitmap.h"
#include <glm/vec2.hpp>
#include <vector>
#include "texture.h"


class Atlas {
 public:
  Atlas(const std::vector<Bitmap>& bitmaps, int tile_size);

  glm::vec2 GetUV(int index) const;
  float GetTileSize() const;
  const Texture& getTexture() const;
 private:
  int tile_size_;
  int pow2_size_;
  float uv_size_;
  //std::unordered_map<char, int> bitmap_indeces_;
  Texture texture_;
};
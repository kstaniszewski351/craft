#pragma once

#include "bitmap.h"
#include <glm/vec2.hpp>
#include <vector>
#include "gfx/texture.h"

int findSize(int n);

class Atlas {
 public:
  Atlas(const std::vector<Bitmap>& bitmaps, int tile_size);
  Atlas(GFX::Texture texture, int pow2_size, int tile_size);
  glm::vec2 GetUV(int index) const;
  glm::ivec2 GetIntegerUV(int index) const;
  int GetIntegerTileSize() const;
  float GetTileSize() const;
  glm::ivec2 GetSize() const;
  const GFX::Texture& getTexture() const;
 private:
  int tile_size_;
  int pow2_size_;
  float uv_size_;
  glm::ivec2 size_;
  GFX::Texture texture_;
};
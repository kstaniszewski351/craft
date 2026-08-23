#pragma once

#include "bitmap.h"
#include <bgfx/bgfx.h>
#include <glm/vec2.hpp>
#include <vector>

int findSize(int n);

class Atlas {
 public:
  Atlas(const std::vector<Bitmap>& bitmaps, int tile_size);
  Atlas(bgfx::TextureHandle texture, int pow2_size, int tile_size);
  glm::vec2 GetUV(int index) const;
  glm::ivec2 GetIntegerUV(int index) const;
  int GetIntegerTileSize() const;
  float GetTileSize() const;
  const bgfx::TextureHandle getTexture() const;
  int GetSize() const;
 private:
  int tile_size_;
  int pow2_size_;
  float uv_size_;
  bgfx::TextureHandle texture_;
};
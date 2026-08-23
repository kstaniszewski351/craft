#include "atlas.h"
#include "bitmap.h"
#include "load_texture.h"
#include <bgfx/bgfx.h>
#include <bgfx/defines.h>
#include <cmath>

bgfx::TextureHandle createTexture(const std::vector<Bitmap>& bitmaps, int tile_size, int pow2_size) {

  int texture_size = pow2_size * tile_size;

  auto bitmap = Bitmap(texture_size, texture_size, bitmaps[0].GetPixelFormat(), bitmaps[0].GetPixelType());


  for (int index = 0; index < bitmaps.size(); index++) {
    int x = (index % (pow2_size)) * tile_size;
    int y = (index / (pow2_size)) * tile_size;
    bitmap.DrawBitmap(x, y, bitmaps[index]);

  }
  auto texture = LoadTexture(bitmap);

  return texture;
}

int findSize(int n_bitmaps) {
    int optimal_size = std::ceil(std::sqrt(n_bitmaps));
  //find next power of 2

  int pow2_size = std::pow(2, std::ceil(std::log(optimal_size)/std::log(2)));
  return pow2_size;
}


Atlas::Atlas(const std::vector<Bitmap>& bitmaps, int tile_size) :
  tile_size_(tile_size),
  pow2_size_(findSize(bitmaps.size())),
  uv_size_(1.0f / pow2_size_),
  texture_(createTexture(bitmaps, tile_size_, pow2_size_)) {

}

Atlas::Atlas(bgfx::TextureHandle texture, int pow2_size, int tile_size) :
  tile_size_(tile_size),
  pow2_size_(pow2_size),
  uv_size_(1.0f / pow2_size),
  texture_(texture) {

}

float Atlas::GetTileSize() const {
  return uv_size_;
}

int Atlas::GetIntegerTileSize() const {
  return tile_size_;
}

int Atlas::GetSize() const {
  return tile_size_ * pow2_size_;
}

glm::ivec2 Atlas::GetIntegerUV(int index) const {
  int x = (index % pow2_size_) * tile_size_;
  int y = (index / pow2_size_) * tile_size_;

  return glm::ivec2(x, y);
}


glm::vec2 Atlas::GetUV(int index) const {
  float x = (float)(index % (pow2_size_)) / (float)pow2_size_;
  float y = (float)(index / (pow2_size_)) / (float)pow2_size_;

  return glm::vec2(x, y);
}

const bgfx::TextureHandle Atlas::getTexture() const {
  return texture_;
}


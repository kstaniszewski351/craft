#pragma once

#include "bgfx/bgfx.h"
#include <unordered_map>
#include <string>


class TextureManager {
 public:
  bgfx::TextureHandle GetTexture(std::string path);
  bgfx::TextureHandle LoadTexture(std::string path);
 private:
  std::unordered_map<std::string, bgfx::TextureHandle> textures_;
};

#pragma once

#include "gfx/texture.h"
#include <memory>
#include <unordered_map>

GFX::Texture LoadTexture(std::string path);

class TextureManager {
 public:
  const GFX::Texture* GetTexture(std::string path);
  const GFX::Texture* LoadTexture(std::string path);
 private:
  std::unordered_map<std::string, std::unique_ptr<GFX::Texture>> textures_;
};

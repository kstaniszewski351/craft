#include "texture_manager.h"
#include "bitmap.h"
#include "gfx/texture.h"
#include <memory>

GFX::Texture LoadTexture(std::string path) {
  Bitmap bitmap(path, PixelFormat::RGBA, PixelType::UInt8);
  GFX::Texture texture(bitmap, 1, {
    GFX::Texture::FilterMode::Nearest,
    GFX::Texture::MipmapMode::Disabled,
    GFX::Texture::WrapMode::Clip
  });

  return std::move(texture);
}

const GFX::Texture* TextureManager::GetTexture(std::string path) {
  auto find = textures_.find(path);
  if(find != textures_.end()) {
    return find->second.get();
  }
  return LoadTexture(path);
}

const GFX::Texture* TextureManager::LoadTexture(std::string path) {
  Bitmap bitmap(path, PixelFormat::RGBA, PixelType::UInt8);

  auto texture = std::make_unique<GFX::Texture>(bitmap, 1, GFX::Texture::Options {
    GFX::Texture::FilterMode::Nearest,
    GFX::Texture::MipmapMode::Disabled,
    GFX::Texture::WrapMode::Clip
  });

  textures_[path] = std::move(texture);

  return textures_[path].get();
};
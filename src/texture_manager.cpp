#include "texture_manager.h"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bitmap.h"

// GFX::Texture2D LoadTexture(std::string path) {
//   Bitmap bitmap(path, PixelFormat::RGBA, PixelType::UInt8);
//   GFX::Texture2D texture(bitmap, 1, {
//     GFX::Texture2D::FilterMode::Nearest,
//     GFX::Texture2D::MipmapMode::Disabled,
//     GFX::Texture2D::WrapMode::Clip
//   });

//   return std::move(texture);
// }

bgfx::TextureHandle TextureManager::GetTexture(std::string path) {
  auto find = textures_.find(path);
  if(find != textures_.end()) {
    return find->second;
  }
  return LoadTexture(path);
}

bgfx::TextureHandle TextureManager::LoadTexture(std::string path) {
  Bitmap bitmap(path, PixelFormat::RGBA, PixelType::UInt8);

  bgfx::TextureHandle texture = bgfx::createTexture(
    bgfx::copy(bitmap.GetPixels(), bitmap.GetSize()),
    BGFX_SAMPLER_MIN_POINT |
    BGFX_SAMPLER_MAG_POINT |
    BGFX_SAMPLER_MIP_POINT
  );
  // auto texture = std::make_unique<GFX::Texture2D>(bitmap, 1, GFX::Texture::Options {
  //   GFX::Texture::FilterMode::Nearest,
  //   GFX::Texture::MipmapMode::Disabled,
  //   GFX::Texture::WrapMode::Clip
  // });

  textures_[path] = texture;

  return texture;
};
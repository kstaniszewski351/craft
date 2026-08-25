#include "texture_manager.h"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bitmap.h"
#include "load_texture.h"

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
  Bitmap bitmap(path, PixelFormat::RGBA, PixelType::UInt8);
  bgfx::TextureHandle texture = LoadTexture(bitmap);
  
  textures_[path] = texture;

  return texture;
}
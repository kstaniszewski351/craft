#include "texture.h"

#include "bitmap.h"
#include "asset_manager.h"
#include "asset.h"
//#include "cubemap.h"
//#include "cubemap_texture.h"
//#include "mesh.h"
#include <cmath>
#include <memory>
#include <array>

constexpr std::array<GLenum, 4> to_gl_format {
  GL_RED,
  GL_RG,
  GL_RGB,
  GL_RGBA
};

constexpr std::array<GLenum, 12> to_gl_internal_format {
  GL_R8,
  GL_RG8,
  GL_RGB8,
  GL_RGBA8,
  GL_R16,
  GL_RG16,
  GL_RGB16,
  GL_RGBA16,
  GL_R16F,
  GL_RG16F,
  GL_RGB16F,
  GL_RGBA16F
};

constexpr std::array<GLenum, 3> to_gl_pixel_type {
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_SHORT,
  GL_FLOAT
};

constexpr std::array<GLenum, 6> to_gl_filter_mode {
  GL_NEAREST,
  GL_LINEAR,
  GL_NEAREST_MIPMAP_NEAREST,
  GL_LINEAR_MIPMAP_NEAREST,
  GL_NEAREST_MIPMAP_LINEAR,
  GL_LINEAR_MIPMAP_LINEAR
};

constexpr std::array<GLenum, 4> to_gl_wrap_mode {
  GL_CLAMP_TO_EDGE,
  GL_CLAMP_TO_BORDER,
  GL_REPEAT,
  GL_MIRRORED_REPEAT
};

constexpr GLenum ToGLInternalPixelFormat(PixelType type, PixelFormat format) {
  return to_gl_internal_format[(int)format + (int)type * 4];
};
constexpr GLenum ToGLPixelFormat(PixelFormat format) {
  return to_gl_format[(int)format];
};
constexpr GLenum ToGLPixelType(PixelType type) {
  return to_gl_pixel_type[(int)type];
};
constexpr GLenum ToGLFilterMode(FilterMode filter_mode, MipmapMode mipmap_mode) {
  return to_gl_filter_mode[(int)mipmap_mode * 2 + (int)filter_mode];
}
constexpr GLenum ToGLWrapMode(WrapMode wrap_mode) {
  return to_gl_wrap_mode[(int)wrap_mode];
}

// std::shared_ptr<Asset> TextureMeta::Load() {
//   std::shared_ptr<Bitmap> bitmap = AssetManager::Get().GetAsset<Bitmap>(bitmap_asset);
//   Bitmap& bitmap_ref = *bitmap.get();
//   auto texture = std::make_shared<Texture>(bitmap_ref);
//   texture->GenMipmaps();
//   return std::static_pointer_cast<Asset>(texture);
// }

Texture::Texture(int width, int height, bool mipmaps, PixelFormat pixel_format, PixelType pixel_type, Texture::Options options)
 : width_(width),
   height_(height),
   pixel_format_(pixel_format),
   pixel_type_(pixel_type) {
  glGenTextures(1, &id_);
  
  UpdateOptions(std::move(options));

  int mip_levels = 1;
  if(mipmaps) {
    mip_levels = GetMaxMipmaps(width, height);
  }
  
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexStorage2D(
    GL_TEXTURE_2D,
    mip_levels,
    ToGLInternalPixelFormat(pixel_type, pixel_format),
    width,
    height
  );
}

int Texture::GetMaxMipmaps(int width, int height) {
  //int resolution = output.GetResolution() * std::pow(0.5, mipmap_level);
  int levels = std::log2(std::max(width, height)) + 1;
}

Texture::Texture(const Bitmap& bitmap, Texture::Options options)
 : width_(bitmap.GetWidth()),
   height_(bitmap.GetHeight()),
   pixel_format_(bitmap.GetPixelFormat()),
   pixel_type_(bitmap.GetPixelType()) {
  
  glGenTextures(1, &id_);
  UpdateOptions(std::move(options));

  GLenum internal_format = ToGLInternalPixelFormat(bitmap.GetPixelType(), bitmap.GetPixelFormat());
  GLenum pixel_format = ToGLPixelFormat(bitmap.GetPixelFormat());
  GLenum pixel_type = ToGLPixelType(bitmap.GetPixelType());

  glBindTexture(GL_TEXTURE_2D, id_);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    internal_format,
    bitmap.GetWidth(),
    bitmap.GetHeight(),
    0,
    pixel_format,
    pixel_type,
    bitmap.GetPixels()
  );
}

Texture::~Texture() {
  glDeleteTextures(1, &id_);
}

void Texture::Bind(int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::GenMipmaps() {
  glGenerateTextureMipmap(id_);
}

int Texture::GetWidth() const {
  return width_;
}
int Texture::GetHeight() const {
  return height_;
}
PixelFormat Texture::GetPixelFormat() const {
  return pixel_format_;
}
PixelType Texture::GetPixelType() const {
  return pixel_type_;
}

GLuint Texture::GetID() const {
  return id_;
}

void Texture::UpdateOptions(Texture::Options options) {
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(options.filter_mode, options.mipmap_mode));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGLFilterMode(options.filter_mode, MipmapMode::Disabled));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGLWrapMode(options.wrap_mode));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGLWrapMode(options.wrap_mode));
}
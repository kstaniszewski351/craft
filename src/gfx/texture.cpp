#include "texture.h"

#include "bitmap.h"
#include "glad/gl.h"
#include <bit>
#include <cstdlib>
#include <array>
#include <algorithm>

namespace GFX {
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
  constexpr PixelFormat ToPixelFormat(GLenum format) {
    return (PixelFormat)(std::distance(to_gl_format.begin(), std::find(to_gl_format.begin(), to_gl_format.end(), format)));
  }
  constexpr GLenum ToGLPixelType(PixelType type) {
    return to_gl_pixel_type[(int)type];
  };
  constexpr PixelType ToPixelType(GLenum type) {
    return (PixelType)(std::distance(to_gl_pixel_type.begin(), std::find(to_gl_pixel_type.begin(), to_gl_pixel_type.end(), type)));
  }
  constexpr GLenum ToGLFilterMode(Texture::FilterMode filter_mode, Texture::MipmapMode mipmap_mode) {
    return to_gl_filter_mode[(int)mipmap_mode * 2 + (int)filter_mode];
  }
  constexpr GLenum ToGLWrapMode(Texture::WrapMode wrap_mode) {
    return to_gl_wrap_mode[(int)wrap_mode];
  }

  int GetMaxMipmaps(unsigned int width, unsigned int height) {
    return std::bit_width(std::max(width, height));
  }

  Texture::Texture(int width, int height, GLenum format, int mip_levels, Texture::Options options) {
    glCreateTextures(GL_TEXTURE_2D, 1, &id_);
    
    UpdateOptions(std::move(options));

    // int mip_levels = 1;
    // if(mipmaps) {
    //   mip_levels = GetMaxMipmaps(width, height);
    // }
    
    glTextureStorage2D(
      id_,
      mip_levels,
      format,
      width,
      height
    );
  }


  Texture::Texture(const Bitmap& bitmap, int mip_levels, Texture::Options options) {
    

    GLenum internal_format = ToGLInternalPixelFormat(bitmap.GetPixelType(), bitmap.GetPixelFormat());
    GLenum pixel_format = ToGLPixelFormat(bitmap.GetPixelFormat());
    GLenum pixel_type = ToGLPixelType(bitmap.GetPixelType());

    // int mip_levels = 1;
    // if(mipmaps) {
    //   mip_levels = GetMaxMipmaps(bitmap.GetWidth(), bitmap.GetHeight());
    // }
    
    glCreateTextures(GL_TEXTURE_2D, 1, &id_);
    UpdateOptions(std::move(options));
    glTextureStorage2D(
      id_,
      mip_levels,
      internal_format,
      bitmap.GetWidth(),
      bitmap.GetHeight()
    );
    glTextureSubImage2D(
      id_,
      0,
      0,
      0,
      bitmap.GetWidth(),
      bitmap.GetHeight(),
      pixel_format,
      pixel_type,
      bitmap.GetPixels()
    );
  }

  Texture::~Texture() {
    glDeleteTextures(1, &id_);
  }

  void Texture::Bind(int slot) const {
    // glActiveTexture(GL_TEXTURE0 + slot);
    // glBindTexture(GL_TEXTURE_2D, id_);
    glBindTextureUnit(slot, id_);
  }

  void Texture::GenMipmaps() {
    glGenerateTextureMipmap(id_);
  }

  int Texture::GetWidth() const {
    int width;
    glGetTextureLevelParameteriv(id_, 0, GL_TEXTURE_WIDTH, &width);
    return width;
  }

  int Texture::GetHeight() const {
    int height;
    glGetTextureLevelParameteriv(id_, 0, GL_TEXTURE_WIDTH, &height);
    return height;
  }

  Bitmap Texture::GetBitmap() const {

    int width = GetWidth();
    int height = GetHeight();
    GLint format = GL_RGBA;
    GLint f1;
    glGetTextureParameteriv(id_, GL_TEXTURE_IMMUTABLE_FORMAT, &f1);
    GLuint type = GL_UNSIGNED_BYTE;

    int size = width * height * 4;
    void* data = std::malloc(size);
    glGetTextureImage(id_, 0, format, type, size, data);

    return Bitmap(width, height, data, ToPixelFormat(format), ToPixelType(type));
  }

  GLuint Texture::GetID() const {
    return id_;
  }

  void Texture::UpdateOptions(Texture::Options options) {
    glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(options.filter_mode, options.mipmap_mode));
    glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, ToGLFilterMode(options.filter_mode, MipmapMode::Disabled));
    glTextureParameteri(id_, GL_TEXTURE_WRAP_S, ToGLWrapMode(options.wrap_mode));
    glTextureParameteri(id_, GL_TEXTURE_WRAP_T, ToGLWrapMode(options.wrap_mode));
  }

  Texture::Texture(Texture&& other) noexcept : id_(other.id_) {
    other.id_ = 0;
  }

  Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        glDeleteTextures(1, &id_);
        id_ = other.id_; 
        other.id_ = 0;
    }
    return *this;
  }
}

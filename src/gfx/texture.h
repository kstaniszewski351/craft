#pragma  once

#include <glad/gl.h>
#include "bitmap.h"

namespace GFX {
  int GetMaxMipmaps(unsigned int width, unsigned int height);

  class Texture {
   public:
    enum class FilterMode : int {
      Nearest = 0,
      Linear
    };

    enum class MipmapMode : int {
      Disabled = 0,
      Nearest,
      Linear
    };

    enum class WrapMode : int {
      Clip = 0,
      Extend,
      Repeat,
      MirrorRepeat
    };
    struct Options {
      FilterMode filter_mode = FilterMode::Linear;
      MipmapMode mipmap_mode = MipmapMode::Disabled;
      WrapMode wrap_mode = WrapMode::Repeat;

      static Options Default() {
        return Options {
          .filter_mode = FilterMode::Linear,
          .mipmap_mode = MipmapMode::Disabled,
          .wrap_mode = WrapMode::Repeat
        };
      };
    };

    Texture(
      int width,
      int height,
      GLenum format,
      int mip_levels = 1,
      Texture::Options options = Texture::Options::Default()
    );
    Texture(
      const Bitmap& bitmap,
      int mip_levels = 1,
      Texture::Options = Texture::Options::Default()
    );
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
    ~Texture();
    void Bind(int slot) const;
    void GenMipmaps();
    void UpdateOptions(Texture::Options options);
    GLuint GetID() const;
    int GetWidth() const;
    int GetHeight() const;
    Bitmap GetBitmap() const;
   private:
    int mipmap_levels_;
    Texture::Options options_;
    GLuint id_;
    
   friend class Framebuffer;
  };
}


#pragma  once

#include <glad/gl.h>
#include <memory>
#include "bitmap.h"
#include "asset_meta.h"

struct TextureMeta : public AssetMeta {
 public:
  std::shared_ptr<Asset> Load() override;
  std::string bitmap_asset;
  bool mipmap = true;
};

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


constexpr GLenum ToGLInternalPixelFormat(PixelType type, PixelFormat format);
constexpr GLenum ToGLPixelFormat(PixelFormat format);
constexpr GLenum ToGLPixelType(PixelType type);
constexpr GLenum ToGLFilterMode(FilterMode filter_mode, MipmapMode mipmap_mode);
constexpr GLenum ToGLWrapMode(WrapMode wrap_mode);



class Texture : public Asset {
 public:

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
    bool mipmaps = false,
    PixelFormat pixel_format = PixelFormat::RGB,
    PixelType pixel_type = PixelType::UInt8,
    Texture::Options options = Texture::Options::Default()
  );
  Texture(
    const Bitmap& bitmap,
    bool mipmaps = true,
    Texture::Options = Texture::Options::Default()
  );
  ~Texture() override;
  void Bind(int slot) const;
  void GenMipmaps();
  void UpdateOptions(Texture::Options options);
  GLuint GetID() const;
  int GetWidth() const;
  int GetHeight() const;
  PixelFormat GetPixelFormat() const;
  PixelType GetPixelType() const;

  //static int GetMaxMipmaps(int width, int height);
 private:
  int width_;
  int height_;
  int mipmap_levels_;
  Texture::Options options_;
  PixelFormat pixel_format_;
  PixelType pixel_type_;
  GLuint id_;
};
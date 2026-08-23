#include "load_texture.h"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bitmap.h"
#include <array>
#include <stdexcept>

constexpr std::array<bgfx::TextureFormat::Enum, 13> TO_BGFX_TEXTURE_FORMAT {
  bgfx::TextureFormat::R8,
  bgfx::TextureFormat::RG8,
  bgfx::TextureFormat::RGB8,
  bgfx::TextureFormat::RGBA8
};

bgfx::TextureHandle LoadTexture(Bitmap& bitmap) {

  if(bitmap.GetPixelType() != PixelType::UInt8) {
    throw std::runtime_error("Unsupported pixel type");
  }

  bgfx::TextureFormat::Enum format = TO_BGFX_TEXTURE_FORMAT[static_cast<int>(bitmap.GetPixelFormat())];

  auto texture = bgfx::createTexture2D(
    bitmap.GetWidth(),
    bitmap.GetHeight(),
    false,
    1,
    format,
    BGFX_TEXTURE_NONE | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT,
    bgfx::copy(bitmap.GetPixels(), bitmap.GetSize())
  );

  return texture;
};

#pragma once

#include "asset_meta.h"
#include <memory>
#include <string>

enum class PixelFormat : int {
  Default = -1,
  R = 0,
  RG,
  RGB,
  RGBA
};
enum class PixelType : int {
  Default = -1,
  UInt8 = 0,
  UInt16,
  Float16
};

class Bitmap {
 public:

  Bitmap(const std::string& path, PixelFormat format = PixelFormat::Default, PixelType pixel_type = PixelType::Default);
  Bitmap(int width, int height, PixelFormat format = PixelFormat::RGB, PixelType pixel_type = PixelType::UInt8);
  Bitmap(int x, int y, int w, int h, const Bitmap& bitmap);
  ~Bitmap();
  
  // Copy constructor and assignment
  Bitmap(const Bitmap& other);
  Bitmap& operator=(const Bitmap& other);
  
  // Move constructor and assignment
  Bitmap(Bitmap&& other) noexcept;
  Bitmap& operator=(Bitmap&& other) noexcept;
  
  int GetWidth() const;
  int GetHeight() const;
  int GetStride() const;
  PixelFormat GetPixelFormat() const;
  PixelType GetPixelType() const;
  const void* GetPixels() const;
  void DrawBitmap(int x, int y, const Bitmap& bitmap);
 private:
  void* data_;
  int width_;
  int height_;
  PixelFormat pixel_format_;
  PixelType pixel_type_;
};
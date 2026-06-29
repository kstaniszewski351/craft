#pragma once

#include <string>

enum class PixelFormat : int {
  R = 0,
  RG,
  RGB,
  RGBA
};
enum class PixelType : int {
  UInt8 = 0,
  UInt16,
  Float16
};

class Bitmap {
 public:

  Bitmap(const std::string& path, PixelFormat format = PixelFormat::RGB, PixelType pixel_type = PixelType::UInt8);
  Bitmap(int width, int height, PixelFormat format = PixelFormat::RGB, PixelType pixel_type = PixelType::UInt8);
  Bitmap(int width, int height, void* data, PixelFormat format = PixelFormat::RGB, PixelType pixel_type = PixelType::UInt8, bool owns = true);
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
  std::size_t GetSize() const;
  PixelFormat GetPixelFormat() const;
  PixelType GetPixelType() const;
  const void* GetPixels() const;
  void DrawBitmap(int x, int y, const Bitmap& bitmap);
  // void Save(std::string path) const;
 private:
  void* data_;
  int width_;
  int height_;
  PixelFormat pixel_format_;
  PixelType pixel_type_;
  bool owns_ = true;
};
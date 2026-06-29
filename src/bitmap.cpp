#include "bitmap.h"

#include <stb_image.h>
#include <stb_image_resize2.h>
#include <stb_image_write.h>
#include <cstdlib>
#include <array>
#include <algorithm>
#include <cstring>
#include <stdexcept>

constexpr std::array<int, 3> pixel_type_sizes {
  1,
  2,
  2
};

Bitmap::Bitmap(const std::string& path, PixelFormat pixel_format, PixelType pixel_type) {
  stbi_set_flip_vertically_on_load(true);
  int n_channels;
  int desired_channels = 0;

  desired_channels = (int)pixel_format + 1;
  pixel_format_ = pixel_format;
  
  switch(pixel_type) {
    case PixelType::UInt8:
     data_ = stbi_load(path.c_str(), &width_, &height_, &n_channels, desired_channels);
     break;
    case PixelType::UInt16:
     data_ = stbi_load_16(path.c_str(), &width_, &height_, &n_channels, desired_channels);
     break;
    case PixelType::Float16:
     data_ = stbi_loadf(path.c_str(), &width_, &height_, &n_channels, desired_channels);
     break;
  }
  if(data_ == NULL) {
    throw std::runtime_error(std::string("Failed to load image: ") + path + " reason: " + stbi_failure_reason());
  }


  pixel_type_ = pixel_type;
}

Bitmap::Bitmap(int width, int height, PixelFormat format, PixelType pixel_type)
    : width_(width), height_(height), pixel_format_(format), pixel_type_(pixel_type) {


  auto size = GetSize();
  data_ = std::malloc(size);
  std::memset(data_, 0, size);
}

Bitmap::Bitmap(int width, int height, void* data, PixelFormat format, PixelType pixel_type, bool owns)
: width_(width),
  height_(height),
  pixel_format_(format),
  pixel_type_(pixel_type),
  owns_(owns) {
  if(data == nullptr) {
    throw std::runtime_error("data is nullptr");
  }
  
  if(owns) {
    auto size = GetSize();
    data_ = std::malloc(width * height * size);
    std::memcpy(data_, data, size);
  }
  else {
    data_ = data;
  }
};


Bitmap::Bitmap(int x, int y, int w, int h, const Bitmap& bitmap) : width_(w), height_(h) {

  throw std::runtime_error("not implemented");
  // stbir_datatype t;
  // stbir_filter f;
  // sbir_edge e;

  // stbi_resize(
  //   bitmap.data_,
  //   bitmap.width_,
  //   bitmap.height_,
  //   bitmap.GetStride(),
  //   data_,
  //   width_,
  //   height_,
  //   GetStride(), stbir_pixel_layout pixel_layout, stbir_datatype data_type, stbir_edge edge, stbir_filter filter)
}

Bitmap::~Bitmap() {
  if(owns_) {
    std::free(data_);
  }
}

Bitmap::Bitmap(const Bitmap& other) 
    : width_(other.width_), height_(other.height_), 
      pixel_format_(other.pixel_format_), pixel_type_(other.pixel_type_), owns_(other.owns_) {
  

  if(owns_) {
    auto size = other.GetSize();
    data_ = std::malloc(size);
    std::memcpy(data_, other.data_, size);
  }
  else {
    data_ = other.data_;
  }
}

Bitmap& Bitmap::operator=(const Bitmap& other) {
  if(this != &other) {
    if(owns_) {
      std::free(data_);
    }
    width_ = other.width_;
    height_ = other.height_;
    pixel_format_ = other.pixel_format_;
    pixel_type_ = other.pixel_type_;
    owns_ = other.owns_;
    

    if(other.owns_) {
      auto size = other.GetSize();
      data_ = std::malloc(size);
      std::memcpy(data_, other.data_, size);
    }
    else {
      data_ = other.data_;
    }
  }
  return *this;
}

Bitmap::Bitmap(Bitmap&& other) noexcept
    : data_(other.data_), width_(other.width_), height_(other.height_),
      pixel_format_(other.pixel_format_), pixel_type_(other.pixel_type_), owns_(other.owns_) {
  other.data_ = nullptr;
}

Bitmap& Bitmap::operator=(Bitmap&& other) noexcept {
  if(this != &other) {
    if(owns_) {
      std::free(data_);
    } 
    data_ = other.data_;
    width_ = other.width_;
    height_ = other.height_;
    pixel_format_ = other.pixel_format_;
    pixel_type_ = other.pixel_type_;
    owns_ = other.owns_;
    other.data_ = nullptr;
  }
  return *this;
}

int Bitmap::GetStride() const {
  int n_values = (int)pixel_format_ + 1;
  int n_bytes = pixel_type_sizes[(int)pixel_type_];

  return n_values * n_bytes * width_;
}

int Bitmap::GetHeight() const {
  return height_;
}

int Bitmap::GetWidth() const {
  return width_;
}

PixelFormat Bitmap::GetPixelFormat() const {
  return pixel_format_;
}

PixelType Bitmap::GetPixelType() const {
  return pixel_type_;
}

const void* Bitmap::GetPixels() const {
  return data_;
}

void Bitmap::DrawBitmap(int x, int y, const Bitmap& bitmap) {

  if(pixel_format_ != bitmap.pixel_format_ || pixel_type_ != bitmap.pixel_type_) {
    throw std::runtime_error("Source and destination bitmaps must have the same pixel format and type");
  }

  int n_channels = (int)pixel_format_ + 1;
  int bytes_per_pixel = pixel_type_sizes[(int)pixel_type_] * n_channels;

  for(int row = 0; row < bitmap.height_; row++) {
    int dest_y = y + row;

    if(dest_y < 0 || dest_y >= height_) {
      continue;
    }

    int start_col = std::max(0, -x);
    int end_col = std::min(bitmap.width_, width_ - x);

    if(start_col >= end_col) {
      continue;
    }

    int copy_width = end_col - start_col;
    int src_offset = (row * bitmap.width_ + start_col) * bytes_per_pixel;
    int dst_offset = (dest_y * width_ + (x + start_col)) * bytes_per_pixel;

    unsigned char* src_ptr = (unsigned char*)bitmap.data_ + src_offset;
    unsigned char* dst_ptr = (unsigned char*)data_ + dst_offset;

    std::memcpy(dst_ptr, src_ptr, copy_width * bytes_per_pixel);
  }
}

// void Bitmap::Save(std::string path) const {
//   if(pixel_type_ != PixelType::UInt8) {
//     throw std::runtime_error("Unsupported format");
//   }
//   stbi_write_png(path.c_str(), width_, height_, int comp, data_, GetStride());
// }

std::size_t Bitmap::GetSize() const {
  int n_channels = (int)pixel_format_ + 1;
  int bytes_per_pixel = pixel_type_sizes[(int)pixel_type_] * n_channels;
  int total_bytes = width_ * height_ * bytes_per_pixel;
  return total_bytes;
}
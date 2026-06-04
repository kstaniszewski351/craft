#pragma once

#include <glad/gl.h>
#include <cstddef>

class Buffer {
 public:
  Buffer();
  Buffer(Buffer&& other) noexcept;
  Buffer& operator=(Buffer&&) noexcept;
  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;
  ~Buffer();
  void Data(std::size_t size, const void* data, GLenum usage = GL_STATIC_DRAW);
  void Bind(GLenum target);
  void BindTarget(GLenum target, int index);
 private:
  GLuint id_;
};
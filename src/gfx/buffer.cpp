#include "buffer.h"
#include <cstddef>

namespace GFX {
  Buffer::Buffer() {
    glCreateBuffers(1, &id_);
  }

  void Buffer::Data(std::size_t size, const void* data, GLenum usage) {
    glNamedBufferData(id_, size, data, usage);
  }

  void Buffer::Bind(GLenum target) const {
    glBindBuffer(target, id_);
  }

  void Buffer::BindTarget(GLenum target, int index) const {
    glBindBufferBase(target, index, id_);
  }

  void Buffer::BindVertexBuffer(int index, std::size_t offset, std::size_t stride) const {
    glBindVertexBuffer(index, id_, offset, stride);
  }

  Buffer::~Buffer() {
    glDeleteBuffers(1, &id_);
  }

  Buffer::Buffer(Buffer&& other) noexcept : id_(other.id_) {
    other.id_ = 0;
  }

  Buffer& Buffer::operator=(Buffer&& other) noexcept {
      if (this != &other) {
          glDeleteBuffers(1, &id_);
          id_ = other.id_;
          other.id_ = 0;
      }
      return *this;
  }
}

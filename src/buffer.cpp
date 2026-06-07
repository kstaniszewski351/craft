#include "buffer.h"
#include <cstddef>

Buffer::Buffer() {
  glCreateBuffers(1, &id_);
}

void Buffer::Data(std::size_t size, const void* data, GLenum usage) {
  glNamedBufferData(id_, size, data, usage);
}

void Buffer::Bind(GLenum target) {
  glBindBuffer(target, id_);
}

void Buffer::BindTarget(GLenum target, int index) {
  glBindBufferBase(target, index, id_);
}

void Buffer::BindVertexBuffer(int index, std::size_t offset, std::size_t stride) {
  glBindVertexBuffer(index, id_, offset, stride);
}

Buffer::~Buffer() {
  glDeleteBuffers(1, &id_);
}

Buffer::Buffer(Buffer&& other) noexcept : id_(other.id_) {
  other.id_ = 0;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
    if (this != &other) {                // proper self-assignment guard
        glDeleteBuffers(1, &id_);        // release current resource
        id_ = other.id_;                  // steal the ID
        other.id_ = 0;                    // leave source in a valid empty state
    }
    return *this;                         // required return
}
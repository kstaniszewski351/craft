#include "vao.h"
#include <glad/gl.h>

VAO::~VAO() {
  glDeleteVertexArrays(1, &id_);
}

VAO::VAO(VAO&& other) noexcept : id_(other.id_) {
  other.id_ = 0;
}

VAO& VAO::operator=(VAO&& other) noexcept {
  if (this != &other) {
      glDeleteVertexArrays(1, &id_);
      id_ = other.id_; 
      other.id_ = 0;
  }
  return *this;
}
void VAO::Bind() {
  glBindVertexArray(id_);
}
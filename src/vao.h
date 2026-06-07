#pragma once
#include <cstddef>
#include <glad/gl.h>

class VAO {
 public:
  struct Attrib {
    unsigned int size;
    GLenum type;
    std::size_t offset;
    bool normalized = false;
  };

  template<typename Iterator>
  VAO(const Iterator begin, const Iterator end, std::size_t stride) {
    glCreateVertexArrays(1, &id_);
    Iterator it = begin;
    int i = 0;
    while(it != end) {
      glVertexArrayAttribFormat(id_, i, it->size, it->type, it->normalized, it->offset);
      glVertexArrayAttribBinding(id_, i, 0);
      glEnableVertexArrayAttrib(id_, i);
      i++;
      it++;
    }
  }
  VAO(VAO&& other) noexcept;
  VAO& operator=(VAO&& other) noexcept;
  void Bind();
  ~VAO();
 private:
  GLuint id_ = 0;
};
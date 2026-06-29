#pragma once

#include "texture.h"
#include <glad/gl.h>

namespace GFX {
  class Framebuffer {
  public:
    Framebuffer();
    Framebuffer(Framebuffer&& other) noexcept;
    Framebuffer& operator=(Framebuffer&&) noexcept;
    ~Framebuffer();
    void Bind(GLenum target);
    void AttachTexture(GLenum attachment, Texture& texture);
  private:
    GLuint id_;
  };
}

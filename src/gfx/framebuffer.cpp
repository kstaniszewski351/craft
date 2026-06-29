#include "framebuffer.h"
#include "glad/gl.h"
#include "texture.h"

namespace GFX {
  Framebuffer::Framebuffer() {
    glCreateFramebuffers(1, &id_);
  }

  Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &id_);
  }

  Framebuffer::Framebuffer(Framebuffer&& other) noexcept : id_(other.id_) {
    other.id_ = 0;
  }

  Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
    if (this != &other) {
      glDeleteFramebuffers(1, &id_);
      id_ = other.id_;
      other.id_ = 0;
    }
    return *this;
  }

  void Framebuffer::Bind(GLenum target) {
    glBindFramebuffer(target, id_);
  }

  void Framebuffer::AttachTexture(GLenum attachment, Texture& texture) {
    glNamedFramebufferTexture(id_, attachment, texture.id_, 0);
  }
}

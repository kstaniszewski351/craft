#include "window.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_video.h>
#include <glm/glm.hpp>

#include <stdexcept>

Window::Window(std::string title, glm::ivec2 size) {
  window_ = SDL_CreateWindow(title.c_str(), size.x, size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if(window_ == nullptr) {
    throw std::runtime_error(std::string("Failed to create window: ") + SDL_GetError());
  }
  

  gl_context_ = SDL_GL_CreateContext(window_);
  if(gl_context_ == nullptr) {
    throw std::runtime_error(std::string("Failed to create OpenGL context: ") + SDL_GetError());
  }
}

void Window::SetSize(glm::ivec2 size) {
  SDL_SetWindowSize(window_, size.x, size.y);
};

void Window::Swap() {
  SDL_GL_SwapWindow(window_);
}

void Window::SetLockCursor(bool value) {
  SDL_SetWindowRelativeMouseMode(window_, value);
}

bool Window::GetLockCursor() const {
  return SDL_GetWindowRelativeMouseMode(window_);
}

glm::ivec2 Window::GetSize() const {
  glm::ivec2 size;
  SDL_GetWindowSize(window_, &size.x, &size.y);
  return size;
}

float Window::GetAspectRatio() const {
  glm::ivec2 size = GetSize();
  return (float)size.x / size.y;
}

glm::vec2 Window::GetRelativeMousePos(glm::vec2 pos) const {
  glm::vec2 size = GetSize();
  return pos / size;
}

Window::~Window() {
  SDL_GL_DestroyContext(gl_context_);
  SDL_DestroyWindow(window_);
}
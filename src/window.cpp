#include "window.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <glm/glm.hpp>

#include <stdexcept>

Window::Window(std::string title, glm::ivec2 size) {
  SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;


  window_ = SDL_CreateWindow(title.c_str(), size.x, size.y, flags);
  if(window_ == nullptr) {
    throw std::runtime_error(std::string("Failed to create window: ") + SDL_GetError());
  }
}

void Window::SetSize(glm::ivec2 size) {
  SDL_SetWindowSize(window_, size.x, size.y);
};

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

Window::Platform Window::GetPlatform() const {
  std::string driver = SDL_GetCurrentVideoDriver();

  if(driver == "wayland") {
    return Platform::Wayland;
  }
  else if(driver == "x11") {
    return Platform::X11;
  }
  else if(driver == "cocoa") {
    return Platform::Cocoa;
  }

  throw std::runtime_error("Running on unknown platform");
};

void* Window::GetNativeHandle() const {
  auto platform = GetPlatform();
  auto props = SDL_GetWindowProperties(window_);
  void* handle;

  switch(platform) {
    case Platform::Wayland:
      handle = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
      break;
    case Platform::X11:
      handle = (void*)(uintptr_t)SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
      break;
    default:
      handle = nullptr;
  }

  return handle;
}

void* Window::GetNativeDisplayType() const {
  void* handle = nullptr;

  auto props = SDL_GetWindowProperties(window_);

  auto platform = GetPlatform();

  void* display;

  switch(platform) {
    case Platform::Wayland:
      display = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
      break;
    case Platform::X11:
      display = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
      break;
    case Platform::Cocoa:
      display = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
    default:
      throw std::runtime_error("Unsupported platform");
      break;
  };

  return display;
}



Window::~Window() {
  //SDL_GL_DestroyContext(gl_context_);
  SDL_DestroyWindow(window_);
}
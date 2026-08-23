#pragma once

#include <glm/ext/vector_int2.hpp>
#include <string>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <glm/vec2.hpp>

class Window {
 public:
  enum class Platform {
    X11,
    Wayland,
    Cocoa,
    Windows
  };


  Window(std::string title, glm::ivec2 size);
  ~Window();
  void SetSize(glm::ivec2 size);
  void SetLockCursor(bool value);
  bool GetLockCursor() const;
  glm::ivec2 GetSize() const;
  float GetAspectRatio() const;
  glm::vec2 GetRelativeMousePos(glm::vec2 pos) const;
  void* GetNativeHandle() const;
  void* GetNativeDisplayType() const;
  Platform GetPlatform() const;

 private:
  SDL_Window* window_;
};
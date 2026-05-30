#pragma once

#include <glm/ext/vector_int2.hpp>
#include <string>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <glm/vec2.hpp>

class Window {
 public:
  Window(std::string title = "Window", glm::ivec2 size = glm::ivec2(1920, 1080));
  ~Window();
  void Swap();
  void SetSize(glm::ivec2 size);
  glm::ivec2 GetSize();
  float GetAspectRatio();
  glm::vec2 GetRelativeMousePos(glm::vec2 pos);

 private:
  SDL_Window* window_;
  SDL_GLContext gl_context_;
};
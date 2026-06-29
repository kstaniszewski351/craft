#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>
#include <glm/glm.hpp>
#include <array>

using Mouse = std::array<bool, 6>;
using Keyboard = std::array<bool, SDL_SCANCODE_COUNT>;

class InputManager {
 public:
  void HandleEvent(const SDL_Event& event);
  void Update();
  void EndFrame();
  const Keyboard& GetKeyboardDowns() const;
  const Mouse& GetMouseDowns() const;

  glm::vec2 GetMouseDelta() const;
  glm::vec2 GetMousePos() const;
 private:
  glm::vec2 mouse_delta = glm::vec2();
  Mouse mouse_ {};
  Keyboard keyboard_ {};
  bool mouse_l;
  bool mouse_r;
};
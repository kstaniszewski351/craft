#include "input_manager.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>

void InputManager::Update() {
  SDL_GetRelativeMouseState(&mouse_delta.x, &mouse_delta.y);

}

void InputManager::HandleEvent(const SDL_Event& event) {
  if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    auto& m_event = event.button;

    mouse_[m_event.button] = true; 
  }
  if(event.type == SDL_EVENT_KEY_DOWN) {
    auto& k_event = event.key;
    keyboard_[k_event.scancode] = true;
  }
}

void InputManager::EndFrame() {
  mouse_.fill(false);
  keyboard_.fill(false);
}

glm::vec2 InputManager::GetMouseDelta() const {
  return mouse_delta;
}

glm::vec2 InputManager::GetMousePos() const {
  glm::vec2 pos;
  SDL_GetMouseState(&pos.x, &pos.y);
  return pos;
}

const Mouse& InputManager::GetMouseDowns() const {
  return mouse_;
}

const Keyboard& InputManager::GetKeyboardDowns() const {
  return keyboard_;
}


InputManager gInputManager;
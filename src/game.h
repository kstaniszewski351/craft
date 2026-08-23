#pragma once

#include "font_manager.h"
#include "gui/gui_renderer.h"
#include "input_manager.h"
#include "texture_manager.h"
#include "window.h"
#include "shader_manager.h"

class Game {
 public:
  void Run();
  static Game& Get();
  Window& GetWindow();
  FontManager& GetFontManager();
  TextureManager& GetTextureManager();
  GUI::GUIRenderer& GetGUIRenderer();
  InputManager& GetInputManager();
  ShaderManager& GetShaderManager();
  ~Game();
 private:
  Window* window_;
  FontManager* font_manager_;
  TextureManager* texture_manager_;
  GUI::GUIRenderer* gui_renderer_;
  InputManager* input_manager_;
  ShaderManager* shader_manager_;
};
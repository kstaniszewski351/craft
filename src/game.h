#pragma once

#include "atlas.h"
#include "font_manager.h"
#include "gui/gui_renderer.h"
#include "input_manager.h"
#include "texture_manager.h"
#include "window.h"
#include <memory>

class Game {
 public:
  void Run();
  static Game& Get();
  Window& GetWindow();
  FontManager& GetFontManager();
  TextureManager& GetTextureManager();
  GUI::GUIRenderer& GetGUIRenderer();
  InputManager& GetInputManager();
 private:
  std::unique_ptr<Window> window_;
  std::unique_ptr<FontManager> font_manager_;
  std::unique_ptr<TextureManager> texture_manager_;
  std::unique_ptr<GUI::GUIRenderer> gui_renderer_;
  std::unique_ptr<InputManager> input_manager_;
};
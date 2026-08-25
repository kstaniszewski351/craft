#pragma once

#include "font_manager.h"
#include "gui/gui_manager.h"
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
  InputManager& GetInputManager();
  ShaderManager& GetShaderManager();
  GUI::GuiManager& GetGuiManager();
  Atlas& GetBlockAtlas();
  Atlas& GetBlockIconAtlas();
  ~Game();
 private:
  Atlas* block_atlas_;
  Atlas* block_icon_atlas;
  Window* window_;
  FontManager* font_manager_;
  TextureManager* texture_manager_;
  InputManager* input_manager_;
  ShaderManager* shader_manager_;
  GUI::GuiManager* gui_manager_;
};
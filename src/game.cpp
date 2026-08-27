#include "game.h"
#include "atlas_builder.h"
#include "block.h"
#include "blocks.h"
#include "font_manager.h"
#include "gui/gui_manager.h"
#include "block_icon_gen.h"
#include "init_bgfx.h"
#include "input_manager.h"
#include "item.h"
#include "registries.h"
#include "scene.h"
#include "shader_manager.h"
#include "texture_manager.h"
#include "views.h"
#include "window.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <bgfx/defines.h>
#include <chrono>
#include <bgfx/bgfx.h>

Atlas* GenBlockAtlas() {
  AtlasBuilder builder;
  for(auto& [_, block] : gBlockRegistry.GetObjects()) {
    block->RegisterTextures(builder);
  }
  return builder.Build();
}

Atlas* GenBlockIconAtlas(Atlas& block_atlas) {
  std::vector<const Block*> blocks;
  for(BlockItem* item : gBlockItems) {
    item->RegisterTex(blocks);
  }

  return GenerateIconAtlas(blocks, 100, block_atlas);
}

Game& Game::Get() {
  static Game instance;
  return instance;
}


void Game::Run() {
  window_ = new Window("Craft", glm::vec2(1920, 1080));
  window_->SetLockCursor(true);
  InitBgfx(*window_);

  std::array<bgfx::ViewId, 2> view_order = {Views::IconGen, Views::Default};
  bgfx::setViewOrder(0, 2, view_order.data());
  bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
  bgfx::setViewMode(0, bgfx::ViewMode::Sequential);

  shader_manager_ = new ShaderManager("shaders/");
  font_manager_ = new FontManager();
  texture_manager_ = new TextureManager();
  input_manager_ = new InputManager();
  gui_manager_ = new GUI::GuiManager();

  //GUI gui;
  bool exit = false;
  RegisterBlocks();
  block_atlas_ = GenBlockAtlas();
  block_icon_atlas = GenBlockIconAtlas(*block_atlas_);
  Scene* scene = new Scene(*block_atlas_);

  auto font = std::unique_ptr<Font>(font_manager_->LoadFont("res/FreeSans.ttf", 40));

  float delta_time = 0.0f;
  //gui_manager_->Recalc(window_->GetSize());
  while(! exit) {
    auto start_time = std::chrono::high_resolution_clock::now();


    
    SDL_Event event;
    //handle SDL events
    while(SDL_PollEvent(&event)) {
      input_manager_->HandleEvent(event);
      switch (event.type) {
        case SDL_EVENT_QUIT:
          exit = true;
          break;
        case SDL_EVENT_WINDOW_RESIZED:
          gui_manager_->Recalc(window_->GetSize());
      }
    }
    input_manager_->Update();
    gui_manager_->Update();

    glm::ivec2 window_size = window_->GetSize();
    bgfx::setViewRect(0, 0 , 0, window_size.x, window_size.y);


    scene->Draw(delta_time, GetWindow());
    gui_manager_->Draw();

    //present on screen
    bgfx::touch(0);
    bgfx::frame();
    //calculate delta time
    input_manager_->EndFrame();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end_time - start_time;
    delta_time = duration.count();
  }

  delete gui_manager_;
  delete scene;
  delete block_atlas_;
  delete block_icon_atlas;
  delete input_manager_;
  delete texture_manager_;
  delete font_manager_;
  delete shader_manager_;
  bgfx::shutdown();
}

Game::~Game() {

}

Window& Game::GetWindow() {
  return *window_;
}

FontManager& Game::GetFontManager() {
  return *font_manager_;
}

TextureManager& Game::GetTextureManager() {
  return *texture_manager_;
}

InputManager& Game::GetInputManager() {
  return *input_manager_;
}

ShaderManager& Game::GetShaderManager() {
  return *shader_manager_;
}

GUI::GuiManager& Game::GetGuiManager() {
  return *gui_manager_;
}

Atlas& Game::GetBlockAtlas() {
  return *block_atlas_;
}

Atlas& Game::GetBlockIconAtlas() {
  return *block_icon_atlas;
}
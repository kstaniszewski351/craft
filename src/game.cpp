#include "game.h"
#include "atlas_builder.h"
#include "block.h"
#include "blocks.h"
#include "font.h"
#include "font_manager.h"
#include "gui/gui_renderer.h"
#include "input_manager.h"
#include "registries.h"
#include "scene.h"
#include "texture_manager.h"
#include "window.h"
#include "init_gl.h"
#include "GL/gl.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <memory>
#include <chrono>

Atlas GenBlockAtlas() {
  AtlasBuilder builder;
  for(auto& [_, block] : gBlockRegistry.GetObjects()) {
    block->RegisterTextures(builder);
  }
  return builder.Build();
}

Game& Game::Get() {
  static Game instance;
  return instance;
}


void Game::Run() {
  window_ = std::make_unique<Window>();
  window_->SetLockCursor(true);
  SDL_HideCursor();
  InitGL();

  font_manager_ = std::make_unique<FontManager>();
  texture_manager_ = std::make_unique<TextureManager>();
  gui_renderer_ = std::make_unique<GUI::GUIRenderer>();
  input_manager_ = std::make_unique<InputManager>();

  //GUI gui;
  bool exit = false;
  RegisterBlocks();
  Atlas atlas = GenBlockAtlas();
  auto scene = Scene(atlas);

  auto font = std::unique_ptr<Font>(font_manager_->LoadFont("res/FreeSans.ttf", 40));

  float delta_time = 0.0f;

  while(! exit) {
    auto start_time = std::chrono::high_resolution_clock::now();


    
    SDL_Event event;

    glm::vec2 mouse_motion = {0.0f, 0.0f};

    //gui.BeginInput();
    //handle SDL events
    while(SDL_PollEvent(&event)) {
      input_manager_->HandleEvent(event);
      switch (event.type) {
        case SDL_EVENT_QUIT:
          exit = true;
          break;
      }
    }
    input_manager_->Update();
    //gui.EndInput();
    glm::ivec2 window_size = window_->GetSize();
    glViewport(0, 0, window_size.x, window_size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //DrawGUI(ctx, block_images, window);
    scene.Draw(delta_time, GetWindow());
    //gui_renderer.Draw(atlas_image, window);
    
    //gui.Draw(delta_time, window);
    //present on screen
    window_->Swap();
    //calculate delta time
    input_manager_->EndFrame();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end_time - start_time;
    delta_time = duration.count();
  }
}

Window& Game::GetWindow() {
  return *window_.get();
}

FontManager& Game::GetFontManager() {
  return *font_manager_.get();
}

TextureManager& Game::GetTextureManager() {
  return *texture_manager_.get();
}
GUI::GUIRenderer& Game::GetGUIRenderer() {
  return *gui_renderer_.get();
}

InputManager& Game::GetInputManager() {
  return *input_manager_.get();
}
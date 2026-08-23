#include "game.h"
#include "atlas_builder.h"
#include "blocks.h"
#include "font_manager.h"
#include "gui/gui_renderer.h"
#include "init_bgfx.h"
#include "input_manager.h"
#include "registries.h"
#include "scene.h"
#include "shader_manager.h"
#include "texture_manager.h"
#include "window.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <bgfx/defines.h>
#include <chrono>
#include <bgfx/bgfx.h>

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
  window_ = new Window("Craft", glm::vec2(1920, 1080));
  window_->SetLockCursor(true);
  //InitGL();
  InitBgfx(*window_);
  //SDL_GLContext gl_context = window_.CreateGLContext();


  //font_manager_ = new FontManager();
  shader_manager_ = new ShaderManager("shaders/");
  font_manager_ = new FontManager();
  texture_manager_ = new TextureManager();
  gui_renderer_ = new GUI::GUIRenderer();
  input_manager_ = new InputManager();

  //GUI gui;
  bool exit = false;
  RegisterBlocks();
  Atlas atlas = GenBlockAtlas();
  Scene* scene = new Scene(atlas);

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
    bgfx::setViewRect(0, 0 , 0, window_size.x, window_size.y);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);

    //DrawGUI(ctx, block_images, window);
    scene->Draw(delta_time, GetWindow());
    //gui_renderer.Draw(atlas_image, window);
    
    //gui.Draw(delta_time, window);
    //present on screen
    bgfx::frame();
    //calculate delta time
    input_manager_->EndFrame();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end_time - start_time;
    delta_time = duration.count();
  }

  delete scene;
  delete input_manager_;
  delete gui_renderer_;
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
GUI::GUIRenderer& Game::GetGUIRenderer() {
  return *gui_renderer_;
}

InputManager& Game::GetInputManager() {
  return *input_manager_;
}

ShaderManager& Game::GetShaderManager() {
  return *shader_manager_;
}
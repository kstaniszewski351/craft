#include "game.h"
#include "block_registry.h"
#include "gui.h"
#include "input_manager.h"
#include "scene.h"
#include "window.h"
#include "init_gl.h"
#include "GL/gl.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>

void Game::Run() {
  Window window;
  InitGL();
  GUI gui;
  bool exit = false;
  RegisterBlocks();
  auto scene = Scene();
  SDL_HideCursor();
  nk_context* ctx = gui.GetCtx();
  
  float delta_time = 0.0f;

  while(! exit) {
    auto start_time = std::chrono::high_resolution_clock::now();


    
    SDL_Event event;

    glm::vec2 mouse_motion = {0.0f, 0.0f};

    gui.BeginInput();
    //handle SDL events
    while(SDL_PollEvent(&event)) {
      gInputManager.HandleEvent(event);
      switch (event.type) {
        case SDL_EVENT_QUIT:
          exit = true;
          break;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
          {
            bool is_down = event.type == SDL_EVENT_KEY_DOWN;
            gui.HandleKey(event.key.key, is_down);
          }
          break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
          {
            bool is_down = event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
            gui.HandleMouseButton(event.button.button, event.button.x, event.button.y, event.button.clicks, is_down);
          }
          break;
      }
    }
    gInputManager.Update();
    gui.EndInput();
    glm::ivec2 window_size = window.GetSize();
    glViewport(0, 0, window_size.x, window_size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(nk_begin(ctx, "Blocks", nk_rect(0, 0, 500, 500), NK_WINDOW_BORDER))  {
      nk_layout_row_static(ctx, 30, 80, 1);
      nk_label(ctx, "aaaaaaa", NK_TEXT_LEFT);
      nk_button_label(ctx, "button");
    }
    nk_end(ctx);
    scene.Draw(delta_time, window);
    gui.Draw(delta_time, window);
    //present on screen
    window.Swap();

    //calculate delta time
    gInputManager.EndFrame();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end_time - start_time;
    delta_time = duration.count();
  }
}
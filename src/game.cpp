#include "game.h"
#include "block_registry.h"
#include "input_manager.h"
#include "scene.h"
#include "window.h"
#include "init_gl.h"
#include <thread>
#include "GL/gl.h"

void Game::Run() {
  Window window;
  InitGL();

  bool exit = false;
  RegisterBlocks();
  auto scene = Scene();
  
  float delta_time = 0.0f;

  while(! exit) {
    auto start_time = std::chrono::high_resolution_clock::now();


    
    SDL_Event event;

    glm::vec2 mouse_motion = {0.0f, 0.0f};

    //handle SDL events
    while(SDL_PollEvent(&event)) {
      gInputManager.HandleEvent(event);
      switch (event.type) {
        case SDL_EVENT_QUIT:
          exit = true;
          break;
      }
    }
    gInputManager.Update();

    glm::ivec2 window_size = window.GetSize();
    glViewport(0, 0, window_size.x, window_size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.Draw(delta_time, window);

    //present on screen
    window.Swap();

    //calculate delta time
    gInputManager.EndFrame();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end_time - start_time;
    delta_time = duration.count();
  }
}
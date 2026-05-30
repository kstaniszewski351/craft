#include "block_registry.h"
#include "camera_data.h"
#include "fpv_camera.h"
#include "scene.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <glad/glad.h>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include "camera_controller.h"
#include "window.h"
#include "input_manager.h"

// SDL_Window* window;
// float window_width = 1920;
// float window_height = 1080;

void GLAPIENTRY MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{

  if(type == GL_DEBUG_TYPE_ERROR) {
    throw std::runtime_error(std::string("Opengl error\n Type: ") + std::to_string(type) + "\nMessage: " + std::string(message));
  }
  
}

void Init() {
  if(!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
    throw std::runtime_error("Failed to load opengl");
  };

  SDL_GL_SetSwapInterval(-1);
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glDebugMessageCallback( MessageCallback, 0 );
}

int main() {
  Window window;
  Init();

  bool exit = false;

  // auto world = World();

  // auto world_render = WorldRenderer(world);
  RegisterBlocks();
  auto scene = Scene();

  auto camera = FPVCamera();
  auto camera_controller = CameraController(camera);

  //setup buffers

  GLuint camera_data_buffer;
  glGenBuffers(1, &camera_data_buffer);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_data_buffer);




  
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
        case SDL_EVENT_WINDOW_RESIZED:
          camera.aspect_ratio = window.GetAspectRatio();
          break;
        case SDL_EVENT_MOUSE_MOTION:
          glm::ivec2 size = window.GetSize();
          mouse_motion.x += event.motion.xrel / size.x;
          mouse_motion.y += event.motion.yrel / size.y;
          break;
      }
    }
    gInputManager.Update();

    // camera_controller.Update(delta_time, mouse_motion);
    // CameraData camera_data = {
    //   .view = camera.GetViewMat(),
    //   .projection = camera.GetProjectionMat(),
    //   .position = camera.position
    // };


    // glBindBuffer(GL_UNIFORM_BUFFER, camera_data_buffer);
    // glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), &camera_data, GL_STATIC_DRAW);

    
    //world_render.Update();
    
    //draw
    glm::ivec2 window_size = window.GetSize();
    glViewport(0, 0, window_size.x, window_size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.Draw(delta_time, window);
    //world_render.Draw();
    //present on screen
    window.Swap();

    //calculate delta time
    gInputManager.EndFrame();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end_time - start_time;
    delta_time = duration.count();
  }
}
#include "init_gl.h"
#include <SDL3/SDL_video.h>
#include <glad/glad.h>
#include <GL/gl.h>
#include <stdexcept>

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

void InitGL() {
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

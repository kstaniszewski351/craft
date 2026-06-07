#pragma once

#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "vao.h"
#include "window.h"
#include "nuklear_config.h"

class GUI {
 public:
  GUI();
  ~GUI();
  void Draw(float delta_time, Window& window);
  void BeginInput();
  void EndInput();
  void HandleKey(SDL_Keycode code, bool is_down);
  void HandleMouseButton(Uint8 code, int x, int y, int clicks, bool is_down);
  nk_context* GetCtx();
 private:
  Bitmap setupAtlas();
  nk_context ctx_;
  nk_buffer v_buf_;
  nk_buffer e_buf_;
  nk_buffer cmd_buf_;
  Buffer vbo_;
  Buffer ebo_;
  VAO vao_;
  Texture atlas_texture_;
  Shader shader_;
  nk_draw_null_texture tex_null_;
  nk_font_atlas atlas_;
  nk_font* font_;
  nk_convert_config convert_config_;
};
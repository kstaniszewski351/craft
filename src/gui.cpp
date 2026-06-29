#include "gui.h"
#include "bitmap.h"
#include "glad/gl.h"
#include "input_manager.h"
#include "gfx/shader.h"
#include "gfx/texture.h"
#include <SDL3/SDL_stdinc.h>
#include <cstddef>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/vector_int2.hpp>

struct GUIVertex {
  glm::vec2 pos;
  glm::vec2 uv;
  std::array<unsigned char, 4> color;
};

constexpr std::array<GFX::VAO::Attrib, 3> GUI_VERTEX_FORMAT {{
  {2, GL_FLOAT, offsetof(GUIVertex, pos)},
  {2, GL_FLOAT, offsetof(GUIVertex, uv)},
  {4, GL_UNSIGNED_BYTE, offsetof(GUIVertex, color), true}
}};

constexpr std::array<nk_draw_vertex_layout_element, 4> VERTEX_LAYOUT {{
  {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, offsetof(GUIVertex, pos)},
  {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, offsetof(GUIVertex, uv)},
  {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, offsetof(GUIVertex, color)},
  {NK_VERTEX_LAYOUT_END}
}};

Bitmap GUI::setupAtlas() {
  nk_font_atlas_init_default(&atlas_);
  nk_font_atlas_begin(&atlas_);
  font_ = nk_font_atlas_add_default(&atlas_, 50, 0);
  int w, h;
  const void* img = nk_font_atlas_bake(&atlas_, &w, &h, NK_FONT_ATLAS_RGBA32);
  Bitmap bitmap(w, h, img, PixelFormat::RGBA, PixelType::UInt8);
  return bitmap;
};

GUI::GUI()
: vao_(GUI_VERTEX_FORMAT.begin(),
  GUI_VERTEX_FORMAT.end(),
  sizeof(GUIVertex)),
  atlas_texture_(
    setupAtlas(),
    1,
    {
      GFX::Texture::FilterMode::Nearest,
      GFX::Texture::MipmapMode::Disabled,
      GFX::Texture::WrapMode::Repeat
    }
  ),
  shader_("res/shaders/gui.frag", "res/shaders/gui.vert") 
  {
  atlas_texture_.GenMipmaps();
  //create texture
  nk_font_atlas_end(&atlas_, nk_handle_ptr(&atlas_texture_), &tex_null_);
  nk_init_default(&ctx_, &font_->handle);
  nk_style_load_all_cursors(&ctx_, atlas_.cursors);
  //nk_style_set_font(&ctx_, &font_->handle);

  nk_buffer_init_default(&v_buf_);
  nk_buffer_init_default(&e_buf_);
  nk_buffer_init_default(&cmd_buf_);


  convert_config_.shape_AA = NK_ANTI_ALIASING_ON;
  convert_config_.line_AA = NK_ANTI_ALIASING_ON;
  convert_config_.vertex_layout = VERTEX_LAYOUT.data();
  convert_config_.vertex_size = sizeof(GUIVertex);
  convert_config_.vertex_alignment = alignof(GUIVertex);
  convert_config_.circle_segment_count = 22;
  convert_config_.curve_segment_count = 22;
  convert_config_.arc_segment_count = 22;
  convert_config_.global_alpha = 1.0f;
  convert_config_.tex_null = tex_null_;
}

GUI::~GUI() {
  nk_buffer_free(&v_buf_);
  nk_buffer_free(&e_buf_);
  nk_buffer_free(&cmd_buf_);
  nk_free(&ctx_);
  nk_font_atlas_clear(&atlas_);
}

void GUI::BeginInput() {
  nk_input_begin(&ctx_);
  glm::ivec2 mouse_pos = gInputManager.GetMousePos();
  nk_input_motion(&ctx_, mouse_pos.x, mouse_pos.y);
}

void GUI::EndInput() {
  nk_input_end(&ctx_);
}

void GUI::HandleKey(SDL_Keycode code, bool is_down) {
  bool ctrl_is_down = false;
  switch(code) {
    case SDLK_LALT:
    case SDLK_RSHIFT:
    case SDLK_LSHIFT:    nk_input_key(&ctx_, NK_KEY_SHIFT, is_down); break;
    case SDLK_DELETE:    nk_input_key(&ctx_, NK_KEY_DEL, is_down); break;
    case SDLK_KP_ENTER:
    case SDLK_RETURN:    nk_input_key(&ctx_, NK_KEY_ENTER, is_down); break;
    case SDLK_TAB:       nk_input_key(&ctx_, NK_KEY_TAB, is_down); break;
    case SDLK_BACKSPACE: nk_input_key(&ctx_, NK_KEY_BACKSPACE, is_down); break;
    case SDLK_HOME:      nk_input_key(&ctx_, NK_KEY_TEXT_START, is_down);
                          nk_input_key(&ctx_, NK_KEY_SCROLL_START, is_down); break;
    case SDLK_END:       nk_input_key(&ctx_, NK_KEY_TEXT_END, is_down);
                          nk_input_key(&ctx_, NK_KEY_SCROLL_END, is_down); break;
    case SDLK_PAGEDOWN:  nk_input_key(&ctx_, NK_KEY_SCROLL_DOWN, is_down); break;
    case SDLK_PAGEUP:    nk_input_key(&ctx_, NK_KEY_SCROLL_UP, is_down); break;
    case SDLK_Z:         nk_input_key(&ctx_, NK_KEY_TEXT_UNDO, is_down && ctrl_is_down); break;
    case SDLK_R:         nk_input_key(&ctx_, NK_KEY_TEXT_REDO, is_down && ctrl_is_down); break;
    case SDLK_C:         nk_input_key(&ctx_, NK_KEY_COPY, is_down && ctrl_is_down); break;
    case SDLK_V:         nk_input_key(&ctx_, NK_KEY_PASTE, is_down && ctrl_is_down); break;
    case SDLK_X:         nk_input_key(&ctx_, NK_KEY_CUT, is_down && ctrl_is_down); break;
    case SDLK_B:         nk_input_key(&ctx_, NK_KEY_TEXT_LINE_START, is_down && ctrl_is_down); break;
    case SDLK_E:         nk_input_key(&ctx_, NK_KEY_TEXT_LINE_END, is_down && ctrl_is_down); break;
    case SDLK_UP:        nk_input_key(&ctx_, NK_KEY_UP, is_down); break;
    case SDLK_DOWN:      nk_input_key(&ctx_, NK_KEY_DOWN, is_down); break;

    case SDLK_ESCAPE:    nk_input_key(&ctx_, NK_KEY_TEXT_RESET_MODE, is_down); break;
    // case SDLK_INSERT:
    //     if (is_down) insert_toggle = !insert_toggle;
    //     if (insert_toggle) {
    //         nk_input_key(&ctx_, NK_KEY_TEXT_INSERT_MODE, is_down);
    //     } else {
    //         nk_input_key(&ctx_, NK_KEY_TEXT_REPLACE_MODE, is_down);
    //     }
    //     break;

    case SDLK_A:
        if (ctrl_is_down)
            nk_input_key(&ctx_,NK_KEY_TEXT_SELECT_ALL, is_down);
        break;
    case SDLK_LEFT:
        if (ctrl_is_down)
            nk_input_key(&ctx_, NK_KEY_TEXT_WORD_LEFT, is_down);
        else nk_input_key(&ctx_, NK_KEY_LEFT, is_down);
        break;
    case SDLK_RIGHT:
        if (ctrl_is_down)
            nk_input_key(&ctx_, NK_KEY_TEXT_WORD_RIGHT, is_down);
        else nk_input_key(&ctx_, NK_KEY_RIGHT, is_down);
        break;
  }
}

nk_context* GUI::GetCtx() {
  return &ctx_;
}

void GUI::HandleMouseButton(Uint8 code, int x, int y, int clicks, bool is_down) {
  switch(code) {
    case SDL_BUTTON_LEFT:
      if(clicks > 1) {
        nk_input_button(&ctx_, NK_BUTTON_DOUBLE, x, y, is_down);
      }
      else {
        nk_input_button(&ctx_, NK_BUTTON_LEFT, x, y, is_down);
      }
      break;
      case SDL_BUTTON_MIDDLE: nk_input_button(&ctx_, NK_BUTTON_MIDDLE, x, y, is_down); break;
      case SDL_BUTTON_RIGHT:  nk_input_button(&ctx_, NK_BUTTON_RIGHT, x, y, is_down); break;
      case SDL_BUTTON_X1:     nk_input_button(&ctx_, NK_BUTTON_X1, x, y, is_down); break;
      case SDL_BUTTON_X2:     nk_input_button(&ctx_, NK_BUTTON_X2, x, y, is_down); break;
  }

}

void GUI::Draw(float delta_time, Window& window) {

  glm::ivec2 window_size = window.GetSize();
  glm::mat4 proj = glm::ortho(0.0f, (float)window_size.x, (float)window_size.y, 0.0f);
  shader_.SetUniform(0, proj);
  shader_.Use();

  nk_convert(&ctx_, &cmd_buf_, &v_buf_, &e_buf_, &convert_config_);
  vbo_.Data(v_buf_.size, v_buf_.memory.ptr);
  ebo_.Data(e_buf_.size, e_buf_.memory.ptr);

  const nk_draw_command* cmd = NULL;
  const nk_draw_index* offset = NULL;
  glEnable(GL_SCISSOR_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  nk_draw_foreach(cmd, &ctx_, &cmd_buf_) {
    if(!cmd->elem_count) continue;
    GFX::Texture* t = (GFX::Texture*)cmd->texture.ptr;
    glScissor(cmd->clip_rect.x, window_size.y - (cmd->clip_rect.y + cmd->clip_rect.h), cmd->clip_rect.w, cmd->clip_rect.h);
    t->Bind(0);
    vao_.Bind();
    ebo_.Bind(GL_ELEMENT_ARRAY_BUFFER);
    vbo_.BindVertexBuffer(0, 0, sizeof(GUIVertex));
    glDrawElements(GL_TRIANGLES, cmd->elem_count, GL_UNSIGNED_SHORT, offset);
    offset += cmd->elem_count;
  };
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);

  nk_clear(&ctx_);
  nk_buffer_clear(&cmd_buf_);
  nk_buffer_clear(&e_buf_);
  nk_buffer_clear(&v_buf_);
}
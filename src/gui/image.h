#include "gfx/texture.h"
#include "gui/gui_renderer.h"
#include "widget.h"
#include "rect.h"

namespace GUI {
  class Image : public Widget {
   public:
    Image(const GFX::Texture* texture, glm::ivec2 pos, glm::ivec2 size, Rect uv = {});
    void Draw(GUIRenderer& renderer) override;
    void SetUV(Rect uv);
   private:
    const GFX::Texture* texture_;
    GFX::Buffer uv_;
  };
}
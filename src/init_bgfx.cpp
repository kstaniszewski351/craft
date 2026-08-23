#include "init_bgfx.h"
#include "window.h"
#include <bgfx/bgfx.h>
#include <stdexcept>
#include <string>
#include <unordered_map>

void InitBgfx(Window& window) {
  bgfx::PlatformData pd;
  if(window.GetPlatform() == Window::Platform::Wayland) {
    pd.type = bgfx::NativeWindowHandleType::Wayland;
  }


  pd.nwh = window.GetNativeHandle();
  pd.ndt = window.GetNativeDisplayType();

  bgfx::Init init;
  init.platformData = pd;

  glm::ivec2 size = window.GetSize();
  init.resolution.width = size.x;
  init.resolution.height = size.y;
  init.resolution.reset = BGFX_RESET_VSYNC;

  bgfx::init(init);
}

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "font.h"

class FontManager {
 public:
  FontManager();
  ~FontManager();
  Font* LoadFont(std::string path, int height) const;
 private:
  FT_Library lib_;
};
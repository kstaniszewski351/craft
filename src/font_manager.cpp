#include "font_manager.h"

#include <freetype/freetype.h>
#include <glm/vec2.hpp>
#include <stdexcept>
#include <string>

FontManager::FontManager() {
  if(FT_Init_FreeType(&lib_)) {
    throw std::runtime_error("Failed to initialize FreeType");
  };
}

FontManager::~FontManager() {
  FT_Done_FreeType(lib_);
}

Font* FontManager::LoadFont(std::string path, int height) const {
  FT_Face face;
  if(FT_New_Face(lib_, path.c_str(), 0, &face)) {
    throw std::runtime_error("Failed to load font: " + path);
  };

  Font* font = new Font(face, height);
  FT_Done_Face(face);
  return font;
} 
#include "font.h"
#include <freetype/freetype.h>
#include <freetype/fttypes.h>
#include <stdexcept>
#include <vector>
#include "atlas.h"
#include "bitmap.h"


Atlas Font::createAtlas(FT_Face& face, int height) {
  FT_Set_Pixel_Sizes(face, 0, height);
  

  std::vector<Bitmap> glyphs;
  characters_.reserve('~' - ' ');
  glyphs.reserve('~' - ' ');

  int max_size = 0;
  int atlas_id = 0;
  for(int c = ' '; c <= '~'; c++) {
    if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      throw std::runtime_error("Failed to load glyph");
    }

    auto& glyph = face->glyph;
    auto& bitmap = glyph->bitmap;


    Bitmap glyph_bitmap = (glyph->bitmap.buffer == nullptr) ? 
      Bitmap(
        0,
        0,
        PixelFormat::R,
        PixelType::UInt8
      ) : 
      Bitmap(
        bitmap.width,
        bitmap.rows,
        bitmap.buffer,
        PixelFormat::R,
        PixelType::UInt8
      );
    max_size = std::max(max_size, (int)bitmap.width);
    max_size = std::max(max_size, (int)bitmap.rows);
    glyphs.push_back(std::move(glyph_bitmap));
    characters_[c] = {
      (int)atlas_id,
      ((int)(glyph->advance.x) >> 6),
      glm::ivec2(bitmap.width, bitmap.rows),
      glm::ivec2(glyph->bitmap_left, glyph->bitmap_top)
    };
    atlas_id++;
  }

  Atlas font_atlas(glyphs, max_size);
  return std::move(font_atlas);
}

Font::Font(FT_Face& face, int height) : atlas_(createAtlas(face, height)), height_(height) { 

}

const Font::Character& Font::GetCharacter(char c) const {
  const auto& find = characters_.find(c);
  if(find == characters_.end()) {
    return characters_.at(' ');
  }
  return find->second;
}

int Font::GetHeight() const {
  return height_;
}

const Atlas& Font::GetAtlas() const {
  return atlas_;
}